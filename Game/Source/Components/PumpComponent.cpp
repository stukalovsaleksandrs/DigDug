// Game
#include "Components/PumpComponent.hpp"
#include "Levels/Level.hpp"
// Engine
#include "Utils.hpp"
#include "Engine/Components/Components.hpp"
#include "Engine/Scene/GameObject.hpp"
#include "Engine/Utils/Timer.hpp"
// Standard
#include <print>


Game::PumpComponent::PumpComponent(Engine::GameObject& owner, Engine::RenderComponent const& playerRenderComponent, Level & level)
    : Component{ owner }
    , m_renderComponent{ *owner.GetComponent<Engine::RenderComponent>() }
    , m_playerRenderComponent{ playerRenderComponent }
    , m_level{ level }
    , m_maxWidthPx{ m_renderComponent.GetSpriteViewDims().x }
{
    m_renderComponent.SetSrcWidth(0.f);
}

void Game::PumpComponent::Update() noexcept
{
    if (not m_active or m_paused) return;
    m_currentWidthPx += m_pxPerSec * Engine::Timer::GetInstance().GetDeltaSec();

    if (auto const [flipMode, degrees]{m_renderComponent.GetSettings()};
        Engine::Utils::NearlyZero(degrees, 1.f))
    {
        if (flipMode == SDL_FLIP_HORIZONTAL)
            m_owner.SetLocalPosition(glm::vec2{-m_currentWidthPx + 0.5f * i32tileSideLengthPx, 0.f});

        m_renderComponent.SetSrcRect(SDL_FRect{
            m_maxWidthPx - m_currentWidthPx, 0.f,
            m_currentWidthPx,
            m_renderComponent.GetSpriteDims().y,
        });

        m_renderComponent.dstDims.x = m_currentWidthPx;
    }
    else
    {
        m_renderComponent.SetSrcRect(SDL_FRect{
            m_maxWidthPx - m_currentWidthPx, 0.f,
            m_currentWidthPx,
            m_renderComponent.GetSpriteDims().y,
        });

        float constexpr absOffset{static_cast<float>(i32tileSideLengthPx)};
        float yOffset{ absOffset };
        if (degrees < 0.f) yOffset *= -1;
        m_owner.SetLocalPosition(glm::vec2{-m_currentWidthPx / m_maxWidthPx * i32tileSideLengthPx + 3, yOffset});

        m_renderComponent.dstDims = {
            m_currentWidthPx,
            m_renderComponent.GetSpriteDims().y
        };
    }
}

void Game::PumpComponent::OnEnable() noexcept
{
    Component::OnEnable();

    m_renderComponent.SetActive(true);

    m_currentWidthPx = 0.f;

    auto const settings{m_playerRenderComponent.GetSettings()};
    m_renderComponent.SetSettings(settings);

    // Horizontal
    if (Engine::Utils::NearlyZero(settings.degrees, 1.f))
    {
        if (settings.flipMode != SDL_FLIP_HORIZONTAL)
            m_owner.SetLocalPosition({0.5f * i32tileSideLengthPx, 0.f});
    }
    else// Vertical
    {
        float constexpr absOffset{static_cast<float>(i32tileSideLengthPx)};
        float yOffset{1.5f * absOffset};
        if (settings.degrees < 0.f) yOffset *= -1;
        m_owner.SetLocalPosition(glm::vec2{-absOffset, yOffset});
    }

    // Getting enemies
    m_enemyRenderComponents = m_level.GetEnemyRenderComponents();
}

void Game::PumpComponent::OnDisable() noexcept
{
    Component::OnDisable();
    m_renderComponent.SetActive(false);
}

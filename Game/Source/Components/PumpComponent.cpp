// Game
#include "Components/PumpComponent.hpp"
// Engine
#include "Utils.hpp"
#include "Engine/Components/Components.hpp"
#include "Engine/Scene/GameObject.hpp"
#include "Engine/Utils/Timer.hpp"
// Standard
#include <print>

Game::PumpComponent::PumpComponent(Engine::GameObject& owner, Engine::RenderComponent const& playerRenderComponent,
    Engine::Sprite & pumpHorizontalSprite, Engine::Sprite & pumpVerticalSprite)
    : Component{ owner }
    , m_renderComponent{ *owner.GetComponent<Engine::RenderComponent>() }
    , m_playerRenderComponent{ playerRenderComponent }
    , m_maxWidthPx{ m_renderComponent.GetSpriteViewDims().x }
    , m_pumpHorizontalSprite{ pumpHorizontalSprite }, m_pumpVerticalSprite{ pumpVerticalSprite }
{
    m_renderComponent.SetSrcWidth(0.f);
}

void Game::PumpComponent::Update() noexcept
{
    if (not m_active) return;
    m_currentWidthPx += m_pxPerSec * Engine::Timer::GetInstance().GetDeltaSec();

    auto const settings{m_renderComponent.GetSettings()};
    if (Engine::Utils::NearlyZero(settings.degrees, 1.f))
    {
        if (settings.flipMode == SDL_FLIP_HORIZONTAL)
            m_owner.SetLocalPosition(glm::vec2{-m_currentWidthPx + 0.5f * tileSideLength, 0.f});

        m_renderComponent.SetSrcRect(SDL_FRect{
            m_maxWidthPx - m_currentWidthPx, 0.f,
            m_currentWidthPx,
            m_renderComponent.GetSpriteViewDims().y,
        });
    }
    else
    {
        // m_owner.SetLocalPosition(glm::vec2{-tileSideLength, 0.f});
    }


    // std::println("{}", m_currentDegrees);

    if (m_currentWidthPx >= m_maxWidthPx)
    {
        SetActive(false);
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
        m_renderComponent.SetSpriteView(Engine::Sprite::View{ &m_pumpHorizontalSprite });
        if (settings.flipMode != SDL_FLIP_HORIZONTAL)
            m_owner.SetLocalPosition({0.5f * tileSideLength, 0.f});
    }
    else// Vertical
    {
        float y{};
        float constexpr absOffset{static_cast<float>(tileSideLength)};
        if (settings.degrees > 0.f)// Bottom
        {
            y = 1.5f * absOffset;
        }
        else// Top
        {
            y = - 1.5f * absOffset;
        }
        m_owner.SetLocalPosition(glm::vec2{-absOffset, y});
    }
}

void Game::PumpComponent::OnDisable() noexcept
{
    Component::OnDisable();
    m_renderComponent.SetActive(false);

}

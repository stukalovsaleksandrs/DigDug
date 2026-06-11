// Game
#include "Components/PumpComponent.hpp"
// Engine
#include "Utils.hpp"
#include "Engine/Components/Components.hpp"
#include "Engine/Scene/GameObject.hpp"
#include "Engine/Utils/Timer.hpp"

Game::PumpComponent::PumpComponent(Engine::GameObject& owner, Engine::RenderComponent const& playerRenderComponent)
    : Component{ owner }
    , m_renderComponent{ *owner.GetComponent<Engine::RenderComponent>() }
    , m_playerRenderComponent{ playerRenderComponent }
    , m_maxWidthPx{ m_renderComponent.GetSpriteViewDims().x }
{
    m_renderComponent.SetSrcWidth(0.f);
}

void Game::PumpComponent::Update() noexcept
{
    if (not m_active) return;
    m_currentWidthPx += m_pxPerSec * Engine::Timer::GetInstance().GetDeltaSec();

    if (m_renderComponent.GetSettings().flipMode == SDL_FLIP_HORIZONTAL)
        m_owner.SetLocalPosition(glm::vec2{-m_currentWidthPx + 0.5f * tileSideLength, 0.f});

    m_renderComponent.SetSrcRect(SDL_FRect{
        m_maxWidthPx - m_currentWidthPx, 0.f,
        m_currentWidthPx,
        m_renderComponent.GetSpriteViewDims().y,
    });

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

    if (settings.flipMode != SDL_FLIP_HORIZONTAL)
        m_owner.SetLocalPosition({0.5f * tileSideLength, 0.f});
}

void Game::PumpComponent::OnDisable() noexcept
{
    Component::OnDisable();
    m_renderComponent.SetActive(false);

}

// Game
#include "Components/PumpComponent.hpp"
// Engine
#include "Engine/Components/Components.hpp"
#include "Engine/Scene/GameObject.hpp"
#include "Engine/Utils/Timer.hpp"

Game::PumpComponent::PumpComponent(Engine::GameObject& owner)
    : Component{ owner }
    , m_renderComponent{ *owner.GetComponent<Engine::RenderComponent>() }
    , m_maxWidthPx{ m_renderComponent.GetSpriteViewDims().x }
{
    m_renderComponent.SetSrcWidth(0.f);
}

void Game::PumpComponent::Update() noexcept
{
    if (not m_active) return;
    m_currentWidthPx += m_pxPerSec * Engine::Timer::GetInstance().GetDeltaSec();
    m_renderComponent.SetSrcRect(SDL_FRect{
        m_maxWidthPx - m_currentWidthPx, 0.f,
        m_currentWidthPx,
        m_renderComponent.GetSpriteViewDims().y,
    });

    if (m_currentWidthPx >= m_maxWidthPx)
    {
        m_active = false;
        m_currentWidthPx = 0.f;
        m_renderComponent.SetActive(false);
    }
}

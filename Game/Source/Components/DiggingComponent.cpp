// Game
#include "Components/DiggingComponent.hpp"
// Engine
#include "Engine/Rendering/Renderer.hpp"
#include "Engine/Scene/GameObject.hpp"

Game::DiggingComponent::DiggingComponent(Engine::GameObject& owner) noexcept
    : Component(owner)
{
    Engine::Renderer::GetInstance().RegisterFunction(m_renderFunction);
}

Game::DiggingComponent::~DiggingComponent() noexcept
{
    Engine::Renderer::GetInstance().UnregisterFunction(m_renderFunction);
}

void Game::DiggingComponent::Render() const noexcept
{
    Engine::Renderer::GetInstance().RenderFilledCircle(m_owner.GetWorldLocation(), 10.f);
}

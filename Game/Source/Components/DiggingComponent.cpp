// Game
#include "Components/DiggingComponent.hpp"
// Engine
#include "Constants.hpp"
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
    static auto constexpr offset{ 0.5f * glm::vec2{tileSideLength, tileSideLength} };
    static float constexpr halfTileSideLength{ 0.5f * tileSideLength };
    Engine::Renderer::GetInstance().RenderFilledCircle(m_owner.GetWorldLocation() + offset, halfTileSideLength);
}

// Game
#include "Components/AIComponent.hpp"
// Engine
#include "Engine/Components/MovementComponent.hpp"

Game::AIComponent::AIComponent(Engine::GameObject& owner)
    : Component{owner}
    , m_movementComponent{ *owner.GetComponent<Engine::MovementComponent>() }
{}

void Game::AIComponent::Update() noexcept
{
    Component::Update();
    m_moveCommand.Execute();
}

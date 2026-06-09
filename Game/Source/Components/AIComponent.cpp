// Game
#include "Components/AIComponent.hpp"
// Engine
#include "Engine/Components/MovementComponent.hpp"
#include "Engine/Components/AnimationComponent.hpp"
#include "FSM/PookaStates.hpp"

Game::AIComponent::AIComponent(Engine::GameObject& owner, Dependencies const& dependencies)
    : PawnComponent{owner, dependencies}
    , m_movementComponent{ *owner.GetComponent<Engine::MovementComponent>() }
    , m_pookaFSM{ [&]
    {
        StateBase::Dependencies const stateDependencies{
            .animationComponent = *owner.GetComponent<Engine::AnimationComponent>(),
            .movementComponent = m_movementComponent,
            .owner = owner,
            .levelManager = m_dependencies.levelManager
        };
        FSM::States states;
        states.emplace(typeid(Pooka::Walking), std::make_unique<Pooka::Walking>(stateDependencies));
        return std::pair(std::move(states), states.at(typeid(Pooka::Walking)).get());
    }() }
{}

void Game::AIComponent::Update() noexcept
{
    Component::Update();
    m_moveCommand.Execute();
}

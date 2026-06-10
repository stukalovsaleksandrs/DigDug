// Game
#include "Components/AIComponent.hpp"
#include "Levels/Level.hpp"
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
            .level = m_dependencies.level
        };
        FSM::States states;
        states.emplace(typeid(Pooka::WanderHorizontally), std::make_unique<Pooka::WanderHorizontally>(stateDependencies));
        states.emplace(typeid(Pooka::WanderVertically), std::make_unique<Pooka::WanderVertically>(stateDependencies));
        return std::pair{std::move(states), states.at(SelectInitialState()).get()};
    }() }
{}

void Game::AIComponent::Update() noexcept
{
    PawnComponent::Update();

    m_pookaFSM.Update();
}

std::type_index Game::AIComponent::SelectInitialState() const noexcept
{
    Grid const& grid{ m_dependencies.level.GetGrid() };
    // Any horizontal neighboring cell is ground -> wandering vertically, else -> wandering horizontally
    // TODO: Check for screen borders
    if (glm::i32vec2 const currentCell{ grid.GetCellFromPoint(m_owner.GetWorldLocation()) };
        grid.IsGround({currentCell.x - 1, currentCell.y}) || grid.IsGround({currentCell.x + 1, currentCell.y}))
    {
        return typeid(Pooka::WanderVertically);
    }

    return typeid(Pooka::WanderHorizontally);
}

// Game
#include "Components/AIComponent.hpp"
#include "Levels/Level.hpp"
// Engine
#include "Engine/Components/MovementComponent.hpp"
#include "Engine/Components/AnimationComponent.hpp"
#include "FSM/AIStates.hpp"

Game::AIComponent::AIComponent(Engine::GameObject& owner, Dependencies const& dependencies)
    : PawnComponent{owner, dependencies, false}
    , m_movementComponent{ *owner.GetComponent<Engine::MovementComponent>() }
    , m_fsm{ [&]
    {
        AI::AIStateBase::Dependencies const stateDependencies{
            .owner = owner,
            .level = m_dependencies.level
        };
        FSM::States states;
        auto addState{[&]<typename StateType>()
        {
            states.emplace(typeid(StateType), std::make_unique<StateType>(stateDependencies));
        }};
        addState.operator()<AI::WanderHorizontally>();
        addState.operator()<AI::WanderVertically>();
        addState.operator()<AI::Chase>();
        auto pPumped{ std::make_unique<AI::Pumped>(stateDependencies) };
        pPumped->BindObserver(m_dependencies.level.GetPlayerFSM());
        states.emplace(typeid(AI::Pumped), std::move(pPumped));
        return std::pair{std::move(states), states.at(SelectInitialState()).get()};
    }() }
{}

void Game::AIComponent::Update() noexcept
{
    PawnComponent::Update();

    m_fsm.Update();
}

void Game::AIComponent::OnCaught() noexcept
{
    m_fsm.ProcessGameAction(EventType::OnCaught);
}

std::type_index Game::AIComponent::SelectInitialState() const noexcept
{
    Grid const& grid{ m_dependencies.level.GetGrid() };
    // Any horizontal neighboring cell is ground -> wandering vertically, else -> wandering horizontally
    // TODO: Check for screen borders
    if (glm::i32vec2 const currentCell{ grid.GetCellFromPoint(m_owner.GetWorldTopLeft()) };
        grid.IsGround({currentCell.x - 1, currentCell.y}) || grid.IsGround({currentCell.x + 1, currentCell.y}))
    {
        return typeid(AI::WanderVertically);
    }

    return typeid(AI::WanderHorizontally);
}

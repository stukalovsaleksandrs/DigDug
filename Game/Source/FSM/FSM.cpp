// Game
#include "FSM/FSM.hpp"
// Engine
#include "Engine/Components/MovementComponent.hpp"
#include "Engine/Components/AnimationComponent.hpp"

Game::StateBase::StateBase(Dependencies const& dependencies) noexcept
    : m_dependencies{ dependencies }
    , m_movementComponent{ *m_dependencies.owner.GetComponent<Engine::MovementComponent>() }
    , m_animationComponent{ *m_dependencies.owner.GetComponent<Engine::AnimationComponent>() }
{}

bool Game::StateBase::IsMoving() const noexcept
{
    return m_movementComponent.IsMoving();
}

Game::FSM::FSM(std::pair<States&&, StateBase*> const& data) noexcept
    : m_states{ std::move(data.first) }
    , m_pCurrentState{ data.second }
{
    assert(m_pCurrentState && "Initial state must be provided");
    m_pCurrentState->OnEnter();
}

void Game::FSM::Update() noexcept
{
    TryChangingState(
        m_pCurrentState->Update()
    );
}

void Game::FSM::TryChangingState(StateType const stateType)
{
    if (!stateType.has_value()) return;
    if (stateType.value() == typeid(*m_pCurrentState)) return;
    m_pCurrentState->OnExit();
    m_pCurrentState = StatesAt(stateType.value());
    m_pCurrentState->OnEnter();
}

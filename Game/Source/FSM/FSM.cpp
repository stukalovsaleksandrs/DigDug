// Game
#include "FSM/FSM.hpp"
// Engine
#include "Engine/InputManager.hpp"

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

void Game::FSM::OnNotify(Engine::Event const event, Engine::Subject const&) noexcept
{
    ProcessGameAction(static_cast<EventType>(event.id));
}

void Game::FSM::ProcessGameAction(EventType const type) noexcept
{
    TryChangingState(
        m_pCurrentState->ProcessGameAction(type)
    );
}

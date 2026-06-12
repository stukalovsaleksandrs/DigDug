// Game
#include "Components/PlayerComponent.hpp"
#include "Levels/Level.hpp"

Game::PlayerComponent::PlayerComponent(Engine::GameObject& owner, Dependencies const& dependencies) noexcept
    : PawnComponent{owner, dependencies}
    , m_playerStateMachine{
        [&]{
            Player::State::PlayerStateBase::Dependencies const stateDependencies{
                .owner = owner,
                .level = m_dependencies.level,
                .fsm = m_playerStateMachine
            };
            // NOTE: Direct construction does not work since it requires copy contructors
            FSM::States states;
            auto addState{[&]<typename StateType>()
            {
                states.emplace(typeid(StateType), std::make_unique<StateType>(stateDependencies));
            }};
            addState.operator()<Player::State::Idle>();
            addState.operator()<Player::State::Walk>();
            addState.operator()<Player::State::Dig>();
            states.emplace(typeid(Player::State::Throw), std::make_unique<Player::State::Throw>(stateDependencies, dependencies.level.GetPumpComponent()));
            addState.operator()<Player::State::Pump>();
            addState.operator()<Player::State::Die>();
            // Returning the states and setting the idle state as the default one
            return std::make_pair(std::move(states), states.at(typeid(Player::State::Idle)).get());
        }()
    }
{}

void Game::PlayerComponent::Update() noexcept
{
    PawnComponent::Update();
    m_playerStateMachine.Update();
}

void Game::PlayerComponent::AddPoints(uint32_t const points) noexcept
{
    m_points += points;
    NotifyObservers(m_onPointsIncreased);
    if (m_points == 5) NotifyObservers(m_onCollected5Points);
}

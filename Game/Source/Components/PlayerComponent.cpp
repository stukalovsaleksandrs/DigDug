// Game
#include "Components/PlayerComponent.hpp"
#include "Commands.hpp"
#include "Levels/Level.hpp"
// Engine
#include "Engine/Components/MovementComponent.hpp"
#include "Engine/Components/AnimationComponent.hpp"
// Standard
#include <print>

Game::PlayerComponent::PlayerComponent(Engine::GameObject& owner, Dependencies const& dependencies) noexcept
    : PawnComponent{owner, dependencies}
    , m_playerStateMachine{
        [&]{
            StateBase::Dependencies const stateDependencies{
                .owner = owner,
                .level = m_dependencies.level
            };
            // NOTE: Direct construction does not work since it requires copy contructors
            FSM::States states;
            states.emplace(typeid(Player::State::Idle), std::make_unique<Player::State::Idle>(stateDependencies));
            states.emplace(typeid(Player::State::Walking), std::make_unique<Player::State::Walking>(stateDependencies));
            states.emplace(typeid(Player::State::Digging), std::make_unique<Player::State::Digging>(stateDependencies));
            // Returning the states and setting the idle state as the default one
            return std::pair(std::move(states), states.at(typeid(Player::State::Idle)).get());
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

void Game::PlayerComponent::Attack() const
{
    m_dependencies.level.EnablePump();
}

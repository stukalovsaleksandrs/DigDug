// Game
#include "FSM/PookaStates.hpp"
#include "Utils.hpp"
// Engine
#include "Engine/Components/AnimationComponent.hpp"

#pragma region StateBase
Game::Pooka::PookaStateBase::PookaStateBase(Dependencies const& dependencies)
    : StateBase{ dependencies } {}
#pragma endregion StateBase

#pragma region WanderHorizontally
Game::Pooka::WanderHorizontally::WanderHorizontally(Dependencies const& dependencies)
    : PookaStateBase{ dependencies }{}

Game::StateType Game::Pooka::WanderHorizontally::Update() noexcept
{
    glm::vec2 const currentLocation{ m_dependencies.owner.GetWorldLocation() };
    if (Engine::Utils::NearlyEqual(currentLocation, m_prevLocation))
        FlipDirection();

    m_prevLocation = currentLocation;

    m_pCurrentCommand->Execute();

    return std::nullopt;
}

void Game::Pooka::WanderHorizontally::OnEnter() noexcept
{
    m_dependencies.animationComponent.ChangeAnimation(
        SDL_FRect{0.f, 0.f,
            static_cast<float>(tileSideLength),
            static_cast<float>(tileSideLength)},
        2
    );
}

void Game::Pooka::WanderHorizontally::FlipDirection() noexcept
{
    if (m_pCurrentCommand == &m_moveLeftCommand)
        m_pCurrentCommand = &m_moveRightCommand;
    else
        m_pCurrentCommand = &m_moveLeftCommand;
}

#pragma endregion WanderHorizontally

#pragma region WanderVertically
Game::Pooka::WanderVertically::WanderVertically(Dependencies const& dependencies)
    : PookaStateBase{ dependencies }{}

Game::StateType Game::Pooka::WanderVertically::Update() noexcept
{
    glm::vec2 const currentLocation{ m_dependencies.owner.GetWorldLocation() };
    if (not Engine::Utils::NearlyZero(currentLocation, 1e-3f) && Engine::Utils::NearlyEqual(currentLocation, m_prevLocation))
        FlipDirection();

    m_prevLocation = currentLocation;

    m_pCurrentCommand->Execute();

    return std::nullopt;
}

void Game::Pooka::WanderVertically::OnEnter() noexcept
{
    m_dependencies.animationComponent.ChangeAnimation(
        SDL_FRect{0.f, 0.f,
            static_cast<float>(tileSideLength),
            static_cast<float>(tileSideLength)},
        2
    );
    m_prevLocation =  m_dependencies.owner.GetWorldLocation();
}

void Game::Pooka::WanderVertically::FlipDirection() noexcept
{
    if (m_pCurrentCommand == &m_moveUpCommand)
        m_pCurrentCommand = &m_moveDownCommand;
    else
        m_pCurrentCommand = &m_moveUpCommand;
}
#pragma endregion WanderVertically


// Game
#include "FSM/PookaStates.hpp"
#include "Utils.hpp"
// Engine
#include "Engine/Components/AnimationComponent.hpp"

#pragma region StateBase
Game::Pooka::PookaStateBase::PookaStateBase(Dependencies const& dependencies)
    : StateBase{ dependencies } {}
#pragma endregion StateBase

#pragma region Walking
Game::Pooka::WanderHorizontally::WanderHorizontally(Dependencies const& dependencies)
    : PookaStateBase{ dependencies }{}

Game::StateType Game::Pooka::WanderHorizontally::Update() noexcept
{
    glm::vec2 const currentLocation{ m_dependencies.owner.GetWorldLocation() };
    if (Engine::Utils::NearlyEqual(currentLocation, m_prevLocation, 0.5f))
    {
        if (m_pCurrentCommand == &m_moveLeftCommand)
        {
            m_pCurrentCommand = &m_moveRightCommand;
        }
        else
        {
            m_pCurrentCommand = &m_moveLeftCommand;
        }
    }
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

#pragma endregion Walking

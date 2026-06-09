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
Game::Pooka::Walking::Walking(Dependencies const& dependencies)
    : PookaStateBase{ dependencies }{}

Game::StateType Game::Pooka::Walking::Update() noexcept
{
    return std::nullopt;
}

void Game::Pooka::Walking::OnEnter() noexcept
{
    m_dependencies.animationComponent.ChangeAnimation(
    SDL_FRect{0.f, 0.f,
        static_cast<float>(tileSideLength),
        static_cast<float>(tileSideLength)},
    2
    );
}

#pragma endregion Walking

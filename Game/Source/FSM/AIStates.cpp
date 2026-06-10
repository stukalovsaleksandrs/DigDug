// Game
#include "FSM/AIStates.hpp"
#include "Utils.hpp"
// Engine
#include "Engine/Components/AnimationComponent.hpp"

#pragma region StateBase
Game::AI::AIStateBase::AIStateBase(Dependencies const& dependencies)
    : StateBase{ dependencies } {}
#pragma endregion StateBase

#pragma region Wander
template<typename Direction>
Game::AI::Wander<Direction>::Wander(Dependencies const& dependencies)
    : AIStateBase{ dependencies }
    , m_moveCommand1{ dependencies.movementComponent,
        std::is_same_v<Direction, Horizontal> ? glm::vec2{-1.f, 0.f} : glm::vec2{0.f, 1.f} }
    , m_moveCommand2{ dependencies.movementComponent,
        std::is_same_v<Direction, Horizontal> ? glm::vec2{1.f, 0.f} : glm::vec2{0.f, -1.f} }
    , m_pCurrentCommand{ &m_moveCommand2 }  // Start moving positive direction
{}

template<typename Direction>
Game::StateType Game::AI::Wander<Direction>::Update() noexcept
{
    glm::vec2 const currentLocation{ m_dependencies.owner.GetWorldLocation() };

    // Flipping direction when facing dead end
    if (Engine::Utils::NearlyEqual(currentLocation, m_prevLocation))
        FlipDirection();

    m_prevLocation = currentLocation;
    m_pCurrentCommand->Execute();

    return std::nullopt;
}

template<typename Direction>
void Game::AI::Wander<Direction>::OnEnter() noexcept
{
    m_dependencies.animationComponent.ChangeAnimation(
        SDL_FRect{0.f, 0.f,
            static_cast<float>(tileSideLength),
            static_cast<float>(tileSideLength)},
        2
    );

    // For vertical movement, initialize prevLocation to current position
    if constexpr (std::is_same_v<Direction, Vertical>)
        m_prevLocation = m_dependencies.owner.GetWorldLocation();
}

template<typename Direction>
void Game::AI::Wander<Direction>::FlipDirection() noexcept
{
    if (m_pCurrentCommand == &m_moveCommand1)
        m_pCurrentCommand = &m_moveCommand2;
    else
        m_pCurrentCommand = &m_moveCommand1;
}

template class Game::AI::Wander<Game::AI::Horizontal>;
template class Game::AI::Wander<Game::AI::Vertical>;
#pragma endregion


#ifndef GAME_POOKA_FSM
#define GAME_POOKA_FSM

// Game
#include "FSM.hpp"
// Engine
#include "Engine/Commands.hpp"

namespace Game::Pooka
{
    class PookaStateBase : public StateBase
    {
    public:
        explicit PookaStateBase(Dependencies const& dependencies);
    };

    class WanderHorizontally final : public PookaStateBase
    {
    public:
        explicit WanderHorizontally(Dependencies const& dependencies);
        StateType Update() noexcept override;
        void OnEnter() noexcept override;
        void OnExit() noexcept override{};

    private:
        Engine::MoveCommand m_moveLeftCommand{ m_dependencies.movementComponent, glm::vec2{-1.f, 0.f} };
        Engine::MoveCommand m_moveRightCommand{ m_dependencies.movementComponent, glm::vec2{1.f, 0.f} };
        Engine::MoveCommand* m_pCurrentCommand{ &m_moveRightCommand };// Not ref. since I couldn't assign to it for some reason
        glm::vec2 m_prevLocation{};

        void FlipDirection() noexcept;
    };

    class WanderVertically final : public PookaStateBase
    {
    public:
        explicit WanderVertically(Dependencies const& dependencies);

        StateType Update() noexcept override;
        void OnEnter() noexcept override;
        void OnExit() noexcept override{};

    private:
        Engine::MoveCommand m_moveUpCommand{ m_dependencies.movementComponent, glm::vec2{0.f, 1.f} };
        Engine::MoveCommand m_moveDownCommand{ m_dependencies.movementComponent, glm::vec2{0.f, -1.f} };
        Engine::MoveCommand* m_pCurrentCommand{ &m_moveDownCommand };// Not ref. since I couldn't assign to it for some reason
        glm::vec2 m_prevLocation{};

        void FlipDirection() noexcept;

    };
}

#endif// GAME_POOKA_FSM

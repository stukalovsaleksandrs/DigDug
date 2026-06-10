#ifndef GAME_AI_FSM
#define GAME_AI_FSM

// Game
#include "FSM.hpp"
// Engine
#include "Engine/Commands.hpp"

namespace Game::AI
{
    class AIStateBase : public StateBase
    {
    public:
        explicit AIStateBase(Dependencies const& dependencies);
    };

    // Direction tags
    struct Horizontal {};
    struct Vertical {};

    template<typename Direction>
    class Wander final : public AIStateBase
    {
    public:
        explicit Wander(Dependencies const& dependencies);
        StateType Update() noexcept override;
        void OnEnter() noexcept override;
        void OnExit() noexcept override {}

    private:
        Engine::MoveCommand m_moveCommand1;
        Engine::MoveCommand m_moveCommand2;
        Engine::MoveCommand* m_pCurrentCommand;
        glm::vec2 m_prevLocation{};

        void FlipDirection() noexcept;
    };

    using WanderHorizontally = Wander<Horizontal>;
    using WanderVertically = Wander<Vertical>;
}

#endif// GAME_AI_FSM
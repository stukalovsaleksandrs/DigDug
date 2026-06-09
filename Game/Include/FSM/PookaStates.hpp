#ifndef GAME_POOKA_FSM
#define GAME_POOKA_FSM

// Game
#include "FSM.hpp"

namespace Game::Pooka
{
    class PookaStateBase : public StateBase
    {
    public:
        explicit PookaStateBase(Dependencies const& dependencies);
    };

    class Walking final : public PookaStateBase
    {
    public:
        explicit Walking(Dependencies const& dependencies);
        StateType Update() noexcept override;
        void OnEnter() noexcept override;
        void OnExit() noexcept override{};
    };
}

#endif// GAME_POOKA_FSM

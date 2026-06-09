#ifndef GAME_PLAYER_STATES
#define GAME_PLAYER_STATES

// Game
#include "FSM.hpp"

namespace Game
{
    class LevelManager;
}

namespace Game::Player::State
{
    class PlayerStateBase : public StateBase
    {
    public:
        explicit PlayerStateBase(Dependencies const& dependencies)
            : StateBase{ dependencies }{}

    protected:
        // true - digging, false - not digging
        [[nodiscard]] bool TryDigging() const noexcept;
    };

    class Idle final : public PlayerStateBase
    {
    public:
        explicit Idle(Dependencies const&) noexcept;
        void OnEnter() noexcept override;
        void OnExit() noexcept override{}
        StateType Update() noexcept override;
    };

    class Walking final : public PlayerStateBase
    {
    public:
        explicit Walking(Dependencies const&) noexcept;
        void OnEnter() noexcept override;
        void OnExit() noexcept override{}
        StateType Update() noexcept override;
    };

    class Digging final : public PlayerStateBase
    {
    public:
        explicit Digging(Dependencies const&) noexcept;
        ~Digging() noexcept override;
        Digging(Digging const&) noexcept = delete;
        Digging(Digging&&) noexcept = delete;
        Digging& operator=(Digging&&) noexcept = delete;
        Digging& operator=(Digging const&) noexcept = delete;

        void OnEnter() noexcept override;
        StateType Update() noexcept override;
        void OnExit() noexcept override{};

    private:
        void Dig() const noexcept;

    };
}

#endif

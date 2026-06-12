#ifndef GAME_PLAYER_STATES
#define GAME_PLAYER_STATES

// Game
#include "FSM.hpp"
// Engine
#include "FSM.hpp"
#include "Engine/InputManager.hpp"

namespace Game
{
    class PlayerComponent;
    class LevelManager;
}

namespace Game::Player::State
{
    class PlayerStateBase : public StateBase
    {
    public:
        explicit PlayerStateBase(Dependencies const& dependencies);

    protected:
        PlayerComponent* m_pPlayerComponent;

        Engine::Action m_keyboardUp{SDL_SCANCODE_W, Engine::InputType::held};
        Engine::Action m_keyboardLeft{SDL_SCANCODE_A, Engine::InputType::held};
        Engine::Action m_keyboardDown{SDL_SCANCODE_S, Engine::InputType::held};
        Engine::Action m_keyboardRight{SDL_SCANCODE_D, Engine::InputType::held};
        Engine::Action m_keyboardPointAction{SDL_SCANCODE_P, Engine::InputType::released};
        Engine::Action m_keyboardAttackAction{SDL_SCANCODE_SPACE, Engine::InputType::held};

        Engine::Action m_gamepadUp{SDL_GAMEPAD_BUTTON_DPAD_UP, Engine::InputType::held};
        Engine::Action m_gamepadLeft{SDL_GAMEPAD_BUTTON_DPAD_LEFT, Engine::InputType::held};
        Engine::Action m_gamepadDown{SDL_GAMEPAD_BUTTON_DPAD_DOWN, Engine::InputType::held};
        Engine::Action m_gamepadRight{SDL_GAMEPAD_BUTTON_DPAD_RIGHT, Engine::InputType::held};

        [[nodiscard]] bool TryDigging() const noexcept;
        void BindMovementInput() noexcept;
        void UnbindMovementInput() const noexcept;
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

        void OnEnter() noexcept override;
        StateType Update() noexcept override;
        void OnExit() noexcept override{};

    };
}

#endif

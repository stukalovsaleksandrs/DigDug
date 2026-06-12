#ifndef GAME_PLAYER_STATES
#define GAME_PLAYER_STATES

// Game
#include "FSM.hpp"
// Engine
#include "FSM.hpp"
#include "Engine/InputManager.hpp"

namespace Game
{
    class PumpComponent;
    class PlayerComponent;
    class LevelManager;
}

namespace Game::Player::State
{
    class PlayerStateBase : public StateBase
    {
    public:
        struct Dependencies final
        {
            Engine::GameObject& owner;// GetWorldLocation is not const
            Level& level;
            FSM& fsm;// For Attack()
        };
        explicit PlayerStateBase(Dependencies const& dependencies);

    protected:
        Dependencies m_dependencies;
        PlayerComponent* m_pPlayerComponent{};
        Engine::MovementComponent& m_movementComponent;
        Engine::AnimationComponent& m_animationComponent;

        Engine::InputAction m_keyboardUp{SDL_SCANCODE_W, Engine::InputType::held};
        Engine::InputAction m_keyboardLeft{SDL_SCANCODE_A, Engine::InputType::held};
        Engine::InputAction m_keyboardDown{SDL_SCANCODE_S, Engine::InputType::held};
        Engine::InputAction m_keyboardRight{SDL_SCANCODE_D, Engine::InputType::held};
        Engine::InputAction m_keyboardPointAction{SDL_SCANCODE_P, Engine::InputType::released};
        Engine::InputAction m_keyboardAttackAction{SDL_SCANCODE_SPACE, Engine::InputType::released};

        Engine::InputAction m_gamepadUp{SDL_GAMEPAD_BUTTON_DPAD_UP, Engine::InputType::held};
        Engine::InputAction m_gamepadLeft{SDL_GAMEPAD_BUTTON_DPAD_LEFT, Engine::InputType::held};
        Engine::InputAction m_gamepadDown{SDL_GAMEPAD_BUTTON_DPAD_DOWN, Engine::InputType::held};
        Engine::InputAction m_gamepadRight{SDL_GAMEPAD_BUTTON_DPAD_RIGHT, Engine::InputType::held};
        Engine::InputAction m_gamepadAttackAction{SDL_SCANCODE_SPACE, Engine::InputType::released};

        [[nodiscard]] bool TryDigging() const noexcept;

        void BindAllInput(FSM&) const noexcept;
        void UnbindAllInput() const noexcept;

        [[nodiscard]] StateType ProcessGameAction(GameAction) noexcept override;
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

    class Attacking final : public PlayerStateBase
    {
    public:
        explicit Attacking(Dependencies const& dependencies, PumpComponent& );

        [[nodiscard]] StateType Update() noexcept override;
        void OnEnter() noexcept override;
        void OnExit() noexcept override;

    private:
        PumpComponent& m_pumpComponent;
        float const m_durationSec{ 0.35f };// How long does attacking state last
        float m_currentSec{};

        void BindAttackInput() const noexcept;
        void UnbindAttackInput() const noexcept;
    };

    class Dying final : public PlayerStateBase
    {
    public:
        explicit Dying(Dependencies const&) noexcept;
        [[nodiscard]] StateType Update() noexcept override{return std::nullopt;};
        void OnEnter() noexcept override{};
        void OnExit() noexcept override;
    };
}

#endif

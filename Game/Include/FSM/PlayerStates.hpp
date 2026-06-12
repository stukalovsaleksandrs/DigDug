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
        struct Dependencies final
        {
            Engine::GameObject& owner;// GetWorldLocation is not const
            Level& level;
            PlayerComponent& playerComponent;
        };
        explicit PlayerStateBase(Dependencies const& dependencies);

    protected:
        Dependencies m_dependencies;
        PlayerComponent* m_pPlayerComponent{};
        Engine::MovementComponent& m_movementComponent;
        Engine::AnimationComponent& m_animationComponent;

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
        Engine::Action m_gamepadAttackAction{SDL_SCANCODE_SPACE, Engine::InputType::held};

        [[nodiscard]] bool TryDigging() const noexcept;
        void BindAllInput(PlayerComponent&) const noexcept;
        void UnbindAllInput() const noexcept;
    };

    class Idle final : public PlayerStateBase
    {
    public:
        explicit Idle(Dependencies const&, PlayerComponent&) noexcept;
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
        explicit Attacking(Dependencies const& dependencies)
            : PlayerStateBase(dependencies){}

        [[nodiscard]] StateType Update() noexcept override;
        void OnEnter() noexcept override;
        void OnExit() noexcept override;

    private:
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

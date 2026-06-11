#ifndef GAME_PLAYER_CONTROLLER
#define GAME_PLAYER_CONTROLLER

// Game
#include "Utils.hpp"
#include "FSM/PlayerStates.hpp"
#include "Grid.hpp"
// Engine
#include "PawnComponent.hpp"
#include "Engine/Core/Observer.hpp"
#include "Engine/InputManager.hpp"

namespace Engine
{
    class AnimationComponent;
}

namespace Game
{
    class PlayerComponent final : public PawnComponent, public Engine::Subject
    {
    public:
        explicit PlayerComponent(Engine::GameObject& owner, Dependencies const&) noexcept;
        ~PlayerComponent() noexcept override;
        PlayerComponent(PlayerComponent&&) noexcept = delete;
        PlayerComponent(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;

        void Update() noexcept override;

        void BindInput() noexcept;
        void UnbindInput() const noexcept;

        [[nodiscard]] uint32_t GetPoints() const noexcept{ return m_points; };
        void AddPoints(uint32_t points) noexcept;

        void Attack() const;

    private:
        uint32_t m_points{};

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

        Engine::Event m_onPointsIncreased{ std::to_underlying(EventType::OnPointsIncreased) };
        Engine::Event m_onCollected5Points{ std::to_underlying(EventType::OnCollected5Points) };

        FSM m_playerStateMachine;

    };
}

#endif// GAME_PLAYER_CONTROLLER

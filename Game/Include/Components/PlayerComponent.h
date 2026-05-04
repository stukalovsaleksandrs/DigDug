#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

// Game
#include "Constants.h"
#include "PlayerStateMachine.h"
// Engine
#include "Engine/Components/ComponentBase.h"
#include "Engine/Core/Observer.h"
#include "Engine/InputManager.h"

namespace Engine
{
    class AnimationComponent;
}

namespace Game
{
    class MovementComponent;
    class PlayerComponent : public Engine::Component, public Engine::Observer
    {
    public:
        Engine::Subject subject;

        explicit PlayerComponent(Engine::GameObject& owner) noexcept;
        ~PlayerComponent() noexcept override;
        PlayerComponent(PlayerComponent&&) noexcept = delete;
        PlayerComponent(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;

        void Update() noexcept override;

        void BindInput() noexcept;
        void UnbindInput() const noexcept;

        void OnNotify(Engine::Event event, Engine::Subject const& caller) noexcept override;

        [[nodiscard]] uint32_t GetPoints() const noexcept{ return m_points; };
        void AddPoints(uint32_t points) noexcept;;

    private:
        uint32_t m_points{};
        Engine::MovementComponent& m_movementComponent;
        Engine::RenderComponent& m_renderComponent;

        Engine::Action m_upAction{SDL_SCANCODE_W, Engine::InputType::held};
        Engine::Action m_leftAction{SDL_SCANCODE_A, Engine::InputType::held};
        Engine::Action m_downAction{SDL_SCANCODE_S, Engine::InputType::held};
        Engine::Action m_rightAction{SDL_SCANCODE_D, Engine::InputType::held};
        Engine::Action m_pointAction{SDL_SCANCODE_P, Engine::InputType::released};

        Engine::Event m_onPointsIncreased{ std::to_underlying(EventType::OnPointsIncreased) };
        Engine::Event m_onCollected5Points{ std::to_underlying(EventType::OnCollected5Points) };

        Player::StateMachine m_stateMachine;

    };
}

#endif

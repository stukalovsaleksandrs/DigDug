#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "ComponentBase.h"
#include "Engine/Core/Observer.h"
#include "Engine/Input/InputManager.h"

namespace DAE::Components
{
    class MovementComponent;
    class PlayerComponent : public Component, public Observer
    {
    public:
        Subject subject;

        explicit PlayerComponent(GameObject& owner) noexcept;
        ~PlayerComponent() noexcept override;
        PlayerComponent(PlayerComponent&&) noexcept = delete;
        PlayerComponent(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;

        void BindInput();
        void UnbindInput() const;

        void OnNotify(Event event, Subject const& caller) noexcept override;

        [[nodiscard]] uint32_t GetPoints() const noexcept{ return m_points; };
        void AddPoints(uint32_t points) noexcept;;

    private:
        uint32_t m_points{};
        MovementComponent& m_movementComponent;

       Input::Action m_upAction{SDL_SCANCODE_W, Input::InputType::held};
       Input::Action m_leftAction{SDL_SCANCODE_A, Input::InputType::held};
       Input::Action m_downAction{SDL_SCANCODE_S, Input::InputType::held};
       Input::Action m_rightAction{SDL_SCANCODE_D, Input::InputType::held};
        Input::Action m_pointAction{SDL_SCANCODE_P, Input::InputType::released};

        Event m_onPointsIncreased{ MakeSDBMHash("OnPointsIncreased") };
        Event m_onCollected5Points{ MakeSDBMHash("OnCollected5Points") };
    };
}

#endif

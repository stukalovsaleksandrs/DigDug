#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "ComponentBase.h"
#include "Core/Observer.h"
#include "Input/InputManager.h"

namespace DAE::Components
{
    class MovementComponent;
    class PlayerComponent : public Component, public Observer
    {
    public:
        explicit PlayerComponent(GameObject& owner);

        void BindInput() const;
        void UnbindInput() const;

        void OnNotify(Event event, Subject const& caller) noexcept override;

    private:
        MovementComponent& m_movementComponent;
        // TODO: Make actions bind and undind themselves in the constructor and destructor
        Input::Action m_upAction{SDL_SCANCODE_W, Input::InputType::pressed};
        Input::Action m_leftAction{SDL_SCANCODE_A, Input::InputType::pressed};
        Input::Action m_downAction{SDL_SCANCODE_S, Input::InputType::pressed};
        Input::Action m_rightAction{SDL_SCANCODE_D, Input::InputType::pressed};
    };
}

#endif

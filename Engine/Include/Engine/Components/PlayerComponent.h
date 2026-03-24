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
        ~PlayerComponent() noexcept override = default;
        PlayerComponent(PlayerComponent&&) noexcept = delete;
        PlayerComponent(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;

        void BindInput() ;
        void UnbindInput();

        void OnNotify(Event event, Subject const& caller) noexcept override;

        [[nodiscard]] uint32_t GetPoints() const noexcept{ return m_points; };
        void AddPoints(uint32_t const points) noexcept;;

    private:
        uint32_t m_points{};
        MovementComponent& m_movementComponent;
        // TODO: Create a binding class that will bind and undind actions in the constructor and destructor(RAII)
        // DAE::Input::Action m_upAction;
        // DAE::Input::Action m_leftAction;
        // DAE::Input::Action m_downAction
        // DAE::Input::Action m_rightAction;

        Event m_onPointsIncreased{ MakeSDBMHash("OnPointsIncreased") };
        Event m_onCollected5Points{ MakeSDBMHash("OnCollected5Points") };
    };
}

#endif

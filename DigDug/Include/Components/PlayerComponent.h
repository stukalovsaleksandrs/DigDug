#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "ComponentBase.h"
#include "Core/Observer.h"

namespace DAE::Components
{
    class MovementComponent;
    class PlayerComponent : public Component, public Observer
    {
    public:
        explicit PlayerComponent(GameObject& owner);

        void BindInput() const;

        void OnNotify(Event event, Subject const& caller) noexcept override;

    private:
        MovementComponent& m_movementComponent;

    };
}

#endif

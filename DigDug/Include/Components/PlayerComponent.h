#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H
#include "ComponentBase.h"

namespace DAE::Components
{
    class MovementComponent;
    class PlayerComponent : public Component
    {
    public:
        explicit PlayerComponent(MovementComponent& movementComponent);
        void BindInput();

    private:
        MovementComponent& m_movementComponent;

    };
}

#endif

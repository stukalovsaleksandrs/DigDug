#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "ComponentBase.h"
#include "Core/Observer.h"

namespace DAE::Components
{
    class LivesComponent : public Component
    {
    public:
        Subject onDamageTaken;

        explicit LivesComponent(GameObject& owner, uint32_t lives) noexcept;

        void TakeDamage() noexcept;

    private:
        uint32_t m_lives{};
        Event m_onDiedEvent;

    };
}

#endif

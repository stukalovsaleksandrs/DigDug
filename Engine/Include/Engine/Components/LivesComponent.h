#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "ComponentBase.h"
#include "Components.h"
#include "Engine/Core/Observer.h"

namespace DAE::Components
{
    // TODO: Just move all of this to PlayerComponent

    class LivesComponent : public Component
    {
    public:
        Subject subject;

        explicit LivesComponent(GameObject& owner, uint32_t lives) noexcept;

        void TakeDamage() noexcept;

        [[nodiscard]] uint32_t GetLives() const noexcept;

    private:
        uint32_t m_lives{};
        Event m_onDiedEvent{MakeSDBMHash("OnDied")},
            m_onDamageTaken{MakeSDBMHash("OnDamageTaken")};

    };
}

#endif

#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

// Engine
#include "Engine/Components/ComponentBase.h"
#include "Engine/Core/Observer.h"

namespace Game
{
    // TODO: Just move all of this to PlayerComponent

    class LivesComponent : public Engine::Component
    {
    public:
        Engine::Subject subject;

        explicit LivesComponent(Engine::GameObject& owner, uint32_t lives) noexcept;

        void TakeDamage() noexcept;

        [[nodiscard]] uint32_t GetLives() const noexcept;

    private:
        uint32_t m_lives{};
        Engine::Event m_onDiedEvent{Engine::MakeSDBMHash("OnDied")},
            m_onDamageTaken{Engine::MakeSDBMHash("OnDamageTaken")};

    };
}

#endif

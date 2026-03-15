#ifndef HEALTH_COMPONENT_H
#define HEALTH_COMPONENT_H

#include "ComponentBase.h"
#include "Core/Observer.h"
#include <utility>

namespace DAE::Components
{
    class LivesComponent : public Component, public Subject
    {
    public:
        explicit LivesComponent(GameObject& owner, uint32_t lives) noexcept;
        ~LivesComponent() noexcept override;
        LivesComponent(const LivesComponent&) noexcept = delete;
        LivesComponent(LivesComponent&&) noexcept = delete;
        LivesComponent& operator=(const LivesComponent&) noexcept = delete;
        LivesComponent& operator=(LivesComponent&&) noexcept = delete;

        void TakeDamage() noexcept;

    private:
        uint32_t m_lives{};
        Event m_onDiedEvent;
        Event const m_subjectDeletedEvent{
            std::to_underlying(CommonEvents::SubjectDeleted)
        };
    };
}

#endif

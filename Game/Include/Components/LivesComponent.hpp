#ifndef LIVES_COMPONENT
#define LIVES_COMPONENT

// Game
#include "Utils.hpp"
// Engine
#include "Engine/Components/ComponentBase.hpp"
#include "Engine/Core/Observer.hpp"

namespace Game
{
    class LivesComponent : public Engine::Component, public Engine::Subject
    {
    public:
        explicit LivesComponent(Engine::GameObject& owner, uint32_t lives) noexcept;

        void TakeDamage() noexcept;

        [[nodiscard]] uint32_t GetLives() const noexcept;

    private:
        uint32_t m_lives{};
        Engine::Event m_onDiedEvent{std::to_underlying(EventType::OnDied)},
            m_onDamageTaken{std::to_underlying(EventType::OnDamageTaken)};

    };
}

#endif// LIVES_COMPONENT

#ifndef DIGDUG_AI_COMPONENT
#define DIGDUG_AI_COMPONENT

// Engine
#include "Engine/Commands.hpp"
#include "Engine/Components/ComponentBase.hpp"

namespace Game
{
    class AIComponent final : public Engine::Component
    {
    public:
        explicit AIComponent(Engine::GameObject& owner);
        void Update() noexcept override;

    private:
        Engine::MovementComponent& m_movementComponent;
        Engine::MoveCommand m_moveCommand{ m_movementComponent, {-1, 0} };

    };
}

#endif// DIGDUG_AI_COMPONENT

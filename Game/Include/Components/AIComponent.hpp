#ifndef DIGDUG_AI_COMPONENT
#define DIGDUG_AI_COMPONENT

// Game
#include "FSM/FSM.hpp"
// Engine
#include "PawnComponent.hpp"
#include "Engine/Commands.hpp"

namespace Game
{
    class AIComponent final : public PawnComponent
    {
    public:
        explicit AIComponent(Engine::GameObject& owner, Dependencies const&);
        void Update() noexcept override;

    private:
        Engine::MovementComponent& m_movementComponent;
        Engine::MoveCommand m_moveCommand{ m_movementComponent, {-1, 0} };
        FSM m_pookaFSM;
    };
}

#endif// DIGDUG_AI_COMPONENT

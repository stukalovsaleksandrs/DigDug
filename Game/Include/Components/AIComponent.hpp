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
        void OnCaught() noexcept;

    private:
        Engine::MovementComponent& m_movementComponent;
        FSM m_fsm;
        Engine::MoveCommand m_moveUpCommand{ m_movementComponent, {0, 1} };
        Engine::MoveCommand m_moveDownCommand{ m_movementComponent, {0, -1} };
        glm::vec2 prevLocation{ -1.f, -1.f };

        [[nodiscard]] std::type_index SelectInitialState() const noexcept;
    };
}

#endif// DIGDUG_AI_COMPONENT

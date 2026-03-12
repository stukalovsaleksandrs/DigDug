#include "Input/Commands.h"
#include "GameObject.h"
#include "Components/MovementComponent.h"

DAE::Input::MoveCommand::MoveCommand(Components::MovementComponent& movementComponent, glm::vec2 const direction)
    : Command()
    , m_movementComponent(movementComponent)
    , m_direction(direction)
{}

void DAE::Input::MoveCommand::Execute() noexcept
{
    m_movementComponent.AddDirection(m_direction);
}

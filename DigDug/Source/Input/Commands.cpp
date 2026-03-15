#include "Input/Commands.h"
#include "Scene/GameObject.h"
#include "Components/MovementComponent.h"

/*******************************************
 * Move command
 *******************************************/

DAE::Input::MoveCommand::MoveCommand(Components::MovementComponent& movementComponent, glm::vec2 const direction)
    : Command()
    , m_movementComponent(movementComponent)
    , m_direction(direction)
{}

void DAE::Input::MoveCommand::Execute() noexcept
{
    m_movementComponent.AddDirection(m_direction);
}

/*******************************************
 * Die command
 *******************************************/

DAE::Input::DieCommand::DieCommand(GameObject& object)
    : m_object(object)
{}

void DAE::Input::DieCommand::Execute() noexcept
{
    m_object.MarkForDeletion();
}

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
    // Well, I want to delete it:)
    // I have a problem that all my game objects
    // belong to the scene.
    // Can I mark it for deletion in here?

}

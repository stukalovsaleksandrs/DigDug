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

DAE::Input::TakeDamageCommand::TakeDamageCommand(Components::LivesComponent& livesComponent)
    : m_livesComponent(livesComponent)
{}

void DAE::Input::TakeDamageCommand::Execute() noexcept
{
    if (!m_canExecute) return;
    m_livesComponent.TakeDamage();

    // To make sure the command does not try using a deleted observer
    if (m_livesComponent.GetLives() == 0)
    {
        m_canExecute = false;
    }
}

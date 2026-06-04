// Engine
#include "Commands.hpp"
#include "Components/MovementComponent.hpp"

/*******************************************
* Move command
 *******************************************/

Engine::MoveCommand::MoveCommand(MovementComponent& movementComponent, glm::vec2 const direction)
    : Command()
    , m_movementComponent{movementComponent}
    , m_direction{direction}
{}

void Engine::MoveCommand::Execute() noexcept
{
    m_movementComponent.AddDirection(m_direction);
}

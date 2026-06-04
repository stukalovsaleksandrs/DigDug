// Game
#include "Commands.hpp"

/*******************************************
 * Die command
 *******************************************/

Game::TakeDamageCommand::TakeDamageCommand(Game::LivesComponent& livesComponent)
    : m_livesComponent(livesComponent)
{}

void Game::TakeDamageCommand::Execute() noexcept
{
    if (!m_canExecute) return;
    m_livesComponent.TakeDamage();

    // To make sure the command does not try using a deleted observer
    if (m_livesComponent.GetLives() == 0)
    {
        m_canExecute = false;
    }
}

/*******************************************
 * Point command
 *******************************************/

Game::PointCommand::PointCommand(PlayerComponent& playerComponent)

    : m_playerComponent(playerComponent)
{}

void Game::PointCommand::Execute() noexcept
{
    m_playerComponent.AddPoints(1);
}

#ifndef GAME_COMMANDS_H
#define GAME_COMMANDS_H
// Engine
#include "Engine/Commands.h"
// Project
#include "Components/LivesComponent.h"
#include "Components/PlayerComponent.h"

namespace Game
{
    /*******************************************
    * Take damage command
    *******************************************/

    class TakeDamageCommand final : public Engine::Command
    {
    public:
        explicit TakeDamageCommand(LivesComponent& livesComponent);
        void Execute() noexcept override;

    private:
        LivesComponent& m_livesComponent;
        bool m_canExecute{ true };

    };

    /*******************************************
     * Point command
     *******************************************/
    // TODO: Fix the crash when player is dead
    class PointCommand final : public Engine::Command
    {
    public:
        explicit PointCommand(PlayerComponent& playerComponent);
        void Execute() noexcept override;

    private:
        PlayerComponent& m_playerComponent;

    };
}

#endif// GAME_COMMANDS_H

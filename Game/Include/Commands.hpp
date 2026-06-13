#ifndef GAME_COMMANDS
#define GAME_COMMANDS
// Engine
#include "Engine/Commands.hpp"
// Project
#include "Components/LivesComponent.hpp"
#include "Components/PlayerComponent.hpp"

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
    class PointCommand final : public Engine::Command
    {
    public:
        explicit PointCommand(PlayerComponent& playerComponent);
        void Execute() noexcept override;

    private:
        PlayerComponent& m_playerComponent;

    };

#pragma region AttackStartCommand
    class AttackStartCommand final : public Engine::Command
    {
    public:
        explicit AttackStartCommand(FSM&);
        void Execute() noexcept override;

    private:
        FSM& m_fsm;

    };
#pragma endregion AttackStartCommand


#pragma region AttackStopCommand
    class AttackStopCommand final : public Engine::Command
    {
    public:
        explicit AttackStopCommand();
        void Execute() noexcept override;

    };
#pragma endregion AttackStopCommand
}

#endif// GAME_COMMANDS

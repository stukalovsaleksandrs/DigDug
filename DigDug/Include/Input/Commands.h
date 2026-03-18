#ifndef COMMANDS_H
#define COMMANDS_H

#include "../Scene/GameObject.h"
#include "Components/LivesComponent.h"
#include "glm/vec2.hpp"

namespace DAE::Components
{
    class MovementComponent;
}

namespace DAE::Input
{
    /*******************************************
     * Command base
     *******************************************/

    class Command
    {
    public:
        Command() = default;
        virtual ~Command() noexcept = default;
        Command(Command const&) noexcept = delete;
        Command(Command&&) noexcept = delete;
        Command& operator=(Command const&) noexcept = delete;
        Command& operator=(Command&&) noexcept = delete;

        virtual void Execute() noexcept = 0;
    };

    /*******************************************
     * Move command
     *******************************************/

    class MoveCommand final : public Command
    {
    public:
        explicit MoveCommand(Components::MovementComponent& movementComponent, glm::vec2 direction);

        void Execute() noexcept override;

    private:
        Components::MovementComponent& m_movementComponent;
        glm::vec2 m_direction{};

    };

    /*******************************************
     * Take damage command
     *******************************************/

    class TakeDamageCommand final : public Command
    {
    public:
        explicit TakeDamageCommand(Components::LivesComponent& livesComponent);
        void Execute() noexcept override;

    private:
        Components::LivesComponent& m_livesComponent;
        bool m_canExecute{ true };

    };

}

#endif // COMMANDS_H

#ifndef COMMANDS_H
#define COMMANDS_H

#include "GameObject.h"
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
     * Movement commands
     *******************************************/

    class MoveCommand final : public Command
    {
    public:
        explicit MoveCommand(Components::MovementComponent& movementComponent, glm::vec2 direction);
        ~MoveCommand() noexcept override = default;
        MoveCommand(MoveCommand const&) noexcept = delete;
        MoveCommand& operator=(MoveCommand const&) noexcept = delete;
        MoveCommand(MoveCommand&&) noexcept = delete;
        MoveCommand& operator=(MoveCommand&&) noexcept = delete;

        void Execute() noexcept override;

    private:
        Components::MovementComponent& m_movementComponent;
        glm::vec2 m_direction{};

    };

}

#endif // COMMANDS_H

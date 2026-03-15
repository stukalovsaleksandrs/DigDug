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
     * Die command
     *******************************************/

    class DieCommand final : public Command
    {
    public:
        explicit DieCommand(GameObject&);
        void Execute() noexcept override;

    private:
        GameObject& m_object;
    };

}

#endif // COMMANDS_H

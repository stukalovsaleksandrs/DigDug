#ifndef ENGINE_COMMANDS
#define ENGINE_COMMANDS

// Engine
#include "Engine/Scene/GameObject.hpp"
// Third-party
#include "glm/vec2.hpp"

namespace Engine
{
    class MovementComponent;
    class PlayerComponent;
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
        explicit MoveCommand(MovementComponent& movementComponent, glm::vec2 direction);

        void Execute() noexcept override;

    private:
        MovementComponent& m_movementComponent;
        glm::vec2 m_direction{};

    };

}

#endif// ENGINE_COMMANDS

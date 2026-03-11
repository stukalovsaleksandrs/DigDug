#ifndef COMMANDS_H
#define COMMANDS_H

#include "CommandBase.h"

namespace DAE::Input
{
    /*******************************************
     * Movement commands
     *******************************************/

    class MoveLeftCommand final : public Command
    {
    public:
        ~MoveLeftCommand() noexcept override = default;
        MoveLeftCommand(MoveLeftCommand const&) noexcept = delete;
        MoveLeftCommand& operator=(MoveLeftCommand const&) noexcept = delete;
        MoveLeftCommand(MoveLeftCommand&&) noexcept = delete;
        MoveLeftCommand& operator=(MoveLeftCommand&&) noexcept = delete;

        void Execute(GameObject&) noexcept override;
    };

    // class MoveRightCommand final : public Command
    // {
    // public:
    //     void Execute() noexcept override;
    // };
    //
    // class MoveUpCommand final : public Command
    // {
    // public:
    //     void Execute() noexcept override;
    // };
    //
    // class MoveDownCommand final : public Command
    // {
    // public:
    //     void Execute() noexcept override;
    // };



}

#endif // COMMANDS_H

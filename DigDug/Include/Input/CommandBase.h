#ifndef COMMANDBASE_H
#define COMMANDBASE_H

namespace DAE{ class GameObject; }

namespace DAE::Input
{
    class Command
    {
    public:
        virtual ~Command() noexcept = default;
        Command(Command const&) noexcept = delete;
        Command(Command&&) noexcept = delete;
        Command& operator=(Command const&) noexcept = delete;
        Command& operator=(Command&&) noexcept = delete;

        virtual void Execute(GameObject&) noexcept = 0;
    };

}

#endif // COMMANDBASE_H

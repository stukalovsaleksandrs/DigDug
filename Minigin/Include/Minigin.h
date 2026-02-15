#ifndef MINIGIN_H
#define MINIGIN_H

#include <functional>
#include <filesystem>

namespace DAE
{
    class Minigin final
    {
    public:
        explicit Minigin(std::filesystem::path const& dataPath);
        ~Minigin();
        Minigin(Minigin const& other) = delete;
        Minigin(Minigin&& other) = delete;
        Minigin& operator=(Minigin const& other) = delete;
        Minigin& operator=(Minigin&& other) = delete;

        void Run(std::function<void()> const& load);
        void RunOneFrame();

    private:
        bool m_quit{};

    };
}

#endif

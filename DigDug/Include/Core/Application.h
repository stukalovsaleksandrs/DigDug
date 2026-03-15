#ifndef MINIGIN_H
#define MINIGIN_H

#include <functional>
#include <filesystem>

namespace DAE
{
    class Application final
    {
    public:
        explicit Application(std::filesystem::path const& dataPath);
        ~Application();
        Application(Application const& other) = delete;
        Application(Application&& other) = delete;
        Application& operator=(Application const& other) = delete;
        Application& operator=(Application&& other) = delete;

        void Run(std::function<void()> const& load);
        void RunOneFrame();

    private:
        bool m_quit{};

    };
}

#endif

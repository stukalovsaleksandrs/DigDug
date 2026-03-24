#ifndef MINIGIN_H
#define MINIGIN_H

#include <functional>
#include <filesystem>

#include "Engine/Achievements.h"
#include "glm/vec2.hpp"



namespace DAE
{
    class Application final
    {
    public:
        explicit Application(std::filesystem::path const& dataPath, glm::ivec2 const& windowResolution);
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

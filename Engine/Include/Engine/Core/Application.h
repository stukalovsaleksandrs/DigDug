#ifndef SE_APPLICATION_H
#define SE_APPLICATION_H

// Third-party
#include "glm/vec2.hpp"
// Standard
#include <functional>
#include <filesystem>

namespace Engine
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

#endif// SE_APPLICATION_H

#ifndef SE_APPLICATION_H
#define SE_APPLICATION_H

// Engine
#include "Engine/Rendering/Renderer.h"
#include "Engine/Sound/LoggingSoundService.h"

// Third-party
#include "glm/vec2.hpp"
// Standard
#include <filesystem>

namespace Engine
{
    class Window
    {
    public:
        explicit Window(glm::uvec2 dims, std::string_view title);
        ~Window();
        Window(Window const&) noexcept = delete;
        Window(Window&&) noexcept = delete;
        Window& operator=(Window const&) noexcept = delete;
        Window& operator=(Window &&) noexcept = delete;

        [[nodiscard]] SDL_Window* Get() const noexcept{ return m_pWindow; };

    private:
        SDL_Window* m_pWindow{};

    };

    class Application
    {
    public:
        explicit Application(
            std::filesystem::path const& resourcePath,
            glm::uvec2 windowDims,
            std::string_view windowTitle);
        virtual ~Application();
        Application(Application const&) noexcept = delete;
        Application(Application &&) noexcept = delete;
        Application& operator=(Application const&) noexcept = delete;
        Application& operator=(Application&&) noexcept = delete;

        void Run();

    protected:
        // Not initialized on stack, because initialization is deferred
        std::unique_ptr<Window> m_pWindow{};

        virtual void Update() = 0;

    private:
        bool m_quit{};

        // TODO: Make the service locator own the services
        std::unique_ptr<ISoundService> m_soundService{
#ifdef NDEBUG
            std::make_unique<DefaultSoundService>()
#else
            std::make_unique<LoggingSoundService>()
#endif
        };

        void RunOneFrame();
    };
}

#endif// SE_APPLICATION_H

#ifndef SE_APPLICATION_H
#define SE_APPLICATION_H

// Engine
#include "Engine/Sound/SDLSoundService.h"
#include "Engine/Sound/LoggingSoundService.h"
#include "Window.h"
// Standard
#include <filesystem>

namespace Engine
{
    class Application
    {
    public:
        explicit Application(
            std::filesystem::path const& resourcePath,
            Window::Data const& data,
            std::string_view windowTitle);
        virtual ~Application();
        Application(Application const&) noexcept = delete;
        Application(Application &&) noexcept = delete;
        Application& operator=(Application const&) noexcept = delete;
        Application& operator=(Application&&) noexcept = delete;

        void Run();
        
        // Must be public for Emscripten version to work
        void RunOneFrame();

    protected:
        // Not initialized on stack, because initialization is deferred
        std::unique_ptr<Window> m_pWindow{};

        virtual void Update() = 0;

    private:
        bool m_quit{};

        // TODO: Make the service locator own the services
        std::unique_ptr<SDLSoundService> m_defaultSoundService{
            std::make_unique<SDLSoundService>()
        };

        std::unique_ptr<LoggingSoundService> m_loggingSoundService{
            std::make_unique<LoggingSoundService>(*m_defaultSoundService.get())
        };
    };
}

#endif// SE_APPLICATION_H

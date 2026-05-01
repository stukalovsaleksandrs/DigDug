#ifndef ENGINE_WINDOW_H
#define ENGINE_WINDOW_H

// Third-party
#include <SDL3/SDL_video.h>
#include "glm/vec2.hpp"
// Standard
#include <string_view>

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
}

#endif// ENGINE_WINDOW_H

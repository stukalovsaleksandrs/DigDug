// Engine
#include "Engine/Core/Window.h"
#include "Engine/Utils/Utils.h"

Engine::Window::Data::Data(glm::vec2 const logicalDims, float const scale) noexcept
    : dims{ logicalDims * scale }
    , logicalDims{ logicalDims }
{}

Engine::Window::Window(glm::uvec2 const dims, std::string_view const title)
   : m_pWindow{
       SDL_CreateWindow(
           title.data(),
           static_cast<int>(dims.x),
           static_cast<int>(dims.y),
           SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
       )
   }
{
    if (!m_pWindow)
        Utils::ThrowSDLError("SDL_CreateWindow error");
}

Engine::Window::~Window()
{
    SDL_DestroyWindow(m_pWindow);
}

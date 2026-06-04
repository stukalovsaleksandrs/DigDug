// Engine
#include "Engine/Core/Window.h"
#include "Engine/Utils/Utils.h"

Engine::Window::Data::Data(glm::vec2 const logicalDims, float const scale) noexcept
    : dims{ logicalDims * scale }
    , logicalDims{ logicalDims }
{}

Engine::Window::Window(Data const& _data, std::string_view const title)
    : data{ _data }
    , m_pWindow{
       SDL_CreateWindow(
           title.data(),
           static_cast<int>(_data.dims.x),
           static_cast<int>(_data.dims.y),
           SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
       )
   }
{
    if (!m_pWindow)
        Utils::ThrowSDLError("SDL_CreateWindow failed");
}

Engine::Window::~Window() noexcept
{
    SDL_DestroyWindow(m_pWindow);
}

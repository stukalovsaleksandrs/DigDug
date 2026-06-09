// Engine
#include "Engine/Core/Window.hpp"
#include "Engine/Utils/Utils.hpp"

Engine::Window::Data::Data(glm::vec2 const logicalDims, float const scale) noexcept
    : physicalDims{ logicalDims * scale }
    , logicalDims{ logicalDims }
{}

Engine::Window::Window(Data const& _data, std::string_view const title)
    : data{ _data }
    , m_pWindow{
       SDL_CreateWindow(
           title.data(),
           static_cast<int>(_data.physicalDims.x),
           static_cast<int>(_data.physicalDims.y),
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

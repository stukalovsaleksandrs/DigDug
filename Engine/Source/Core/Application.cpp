// Engine
#include "Core/Application.h"
#include "InputManager.h"
#include "Core/ResourceManager.h"
#include "Rendering/Renderer.h"
#include "Utils/Timer.h"
#include "Utils/Utils.h"
#include "Achievements/Steamworks.h"
// Third-party
#include <SDL3/SDL.h>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
    static_cast<Engine::Application*>(arg)->RunOneFrame();
}
#endif
Engine::Window::Window(glm::uvec2 const dims, std::string_view const title)
    : m_pWindow{
        SDL_CreateWindow(
            title.data(),
            static_cast<int>(dims.x),
            static_cast<int>(dims.y),
            SDL_WINDOW_VULKAN
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

Engine::Application::Application(
    std::filesystem::path const& resourcePath,
    glm::uvec2 const windowDims,
    std::string_view windowTitle)
{
    InitializeSteamWorks();
    Utils::PrintSDLVersion();

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_Log("Renderer error: %s", SDL_GetError());
        Utils::ThrowSDLError("SDL_Init Error");
    }

    m_pWindow = std::make_unique<Window>(windowDims, windowTitle);

    ResourceManager::GetInstance().Init(resourcePath);

    Renderer::GetInstance().Init(m_pWindow.get()->Get());
}

Engine::Application::~Application()
{
    SDL_Quit();
    ShutdownSteamWorks();
}

void Engine::Application::Run()
{
#ifndef __EMSCRIPTEN__
    while (!m_quit)
        RunOneFrame();
#else
    emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void Engine::Application::RunOneFrame()
{
#if USE_STEAMWORKS
    SteamAPI_RunCallbacks();
#endif
    Timer::GetInstance().Update();
    m_quit = !InputManager::GetInstance().ProcessInput();
    Update();
    Renderer::GetInstance().Render();
}

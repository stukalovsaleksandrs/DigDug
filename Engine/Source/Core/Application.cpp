// Engine
#include "Utils/Timer.hpp"
#include "Utils/Utils.hpp"
#include "Core/Application.hpp"
#include "InputManager.hpp"
#include "Rendering/Renderer.hpp"
#include "Achievements/Steamworks.hpp"
#include "Sound/SoundServiceLocator.hpp"
#include "Engine/Core/Window.hpp"
// Third-party
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
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

Engine::Application::Application(
    Window::Data const& data,
    std::string_view windowTitle)
{
    // Steam
    InitializeSteamworks();

    // SDL instance
    Utils::PrintSDLVersion();
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        SDL_Log("Renderer error: %s", SDL_GetError());
        Utils::ThrowSDLError("SDL_Init Error");
    }

    // Window
    m_pWindow = std::make_unique<Window>(data, windowTitle);

    // Renderer
    Renderer::GetInstance().Init(*m_pWindow);

    // TTF
    Utils::Check(
        TTF_Init(),
        "Failed to load SDL_ttf"
    );

    // Sound
#ifndef NDEBUG
    SoundServiceLocator::SetSoundService(*m_loggingSoundService);
#else
    SoundServiceLocator::SetSoundService(*m_defaultSoundService.get());
#endif
}

Engine::Application::~Application()
{
    m_defaultSoundService.reset();
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

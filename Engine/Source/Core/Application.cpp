// Engine
#include "Utils/Timer.h"
#include "Utils/Utils.h"
#include "Core/Application.h"
#include "InputManager.h"
#include "Core/ResourceManager.h"
#include "Rendering/Renderer.h"
#include "Achievements/Steamworks.h"
#include "Sound/SoundServiceLocator.h"
#include "Engine/Core/Window.h"
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

Engine::Application::Application(
    std::filesystem::path const& resourcePath,
    glm::uvec2 const windowDims, glm::uvec2 const windowLogicalDims,
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
    m_pWindow = std::make_unique<Window>(windowDims, windowTitle);

    // Resource manager
    ResourceManager::GetInstance().Init(resourcePath);

    // Renderer
    Renderer::GetInstance().Init(m_pWindow->Get(), windowLogicalDims);

    // Sound
#ifndef NDEBUG
    SoundServiceLocator::SetSoundService(*m_loggingSoundService);
#else
    SoundServiceLocator::SetSoundService(*m_defaultSoundService.get());
#endif
    auto& soundService{ SoundServiceLocator::GetSoundService() };
    auto const soundId{ soundService.LoadSound("Resources/GameStart.mp3") };
    soundService.SetVolume(0.2f);
    soundService.PlaySound(soundId);
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

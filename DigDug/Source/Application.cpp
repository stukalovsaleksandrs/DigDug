#include "Application.h"
#include "../Include/Input/InputManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Timer.h"
#include "Utils.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <sstream>
#include <iostream>

#if WIN32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif

SDL_Window* g_window{};

void LogSDLVersion(std::string_view const message, int const major, int const minor, int const patch)
{
#if WIN32
    std::stringstream ss;
    ss << message << major << "." << minor << "." << patch << "\n";
    OutputDebugString(ss.str().c_str());
#else
    std::cout << message << major << "." << minor << "." << patch << "\n";
#endif
}

#ifdef __EMSCRIPTEN__
#include "emscripten.h"

void LoopCallback(void* arg)
{
    static_cast<DAE::Application*>(arg)->RunOneFrame();
}
#endif

// Why bother with this? Because sometimes students have a different SDL version installed on their pc.
// That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
// These entries in the debug output help to identify that issue.
void PrintSDLVersion()
{
    LogSDLVersion("Compiled with SDL", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION);
    int version{ SDL_GetVersion() };
    LogSDLVersion("Linked with SDL ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
    // LogSDLVersion("Compiled with SDL_image ",SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_MICRO_VERSION);
    // version = IMG_Version();
    // LogSDLVersion("Linked with SDL_image ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version), SDL_VERSIONNUM_MICRO(version));
    LogSDLVersion("Compiled with SDL_ttf ",	SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION,SDL_TTF_MICRO_VERSION);
    version = TTF_Version();
    LogSDLVersion("Linked with SDL_ttf ", SDL_VERSIONNUM_MAJOR(version), SDL_VERSIONNUM_MINOR(version),	SDL_VERSIONNUM_MICRO(version));
}

DAE::Application::Application(std::filesystem::path const& dataPath)
{
    PrintSDLVersion();

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO))
    {
        SDL_Log("Renderer error: %s", SDL_GetError());
        Utils::ThrowSDLError("SDL_Init Error");
    }

    g_window = SDL_CreateWindow(
        "Programming 4 assignment",
        1024,
        576,
        SDL_WINDOW_OPENGL
    );
    if (!g_window)
    {
        Utils::ThrowSDLError("SDL_CreateWindow error");
    }

    Renderer::GetInstance().Init(g_window);
    ResourceManager::GetInstance().Init(dataPath);
}

DAE::Application::~Application()
{
    Renderer::GetInstance().Destroy();
    SDL_DestroyWindow(g_window);
    g_window = nullptr;
    SDL_Quit();
}

void DAE::Application::Run(std::function<void()> const& load)
{
    load();
#ifndef __EMSCRIPTEN__
    while (!m_quit)
        RunOneFrame();
#else
    emscripten_set_main_loop_arg(&LoopCallback, this, 0, true);
#endif
}

void DAE::Application::RunOneFrame()
{
    Timer::GetInstance().Update();
    m_quit = !Input::InputManager::ProcessInput();
    SceneManager::GetInstance().Update();
    Renderer::GetInstance().Render();
}

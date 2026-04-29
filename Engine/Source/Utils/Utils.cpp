// Engine
#include "Engine/Utils/Utils.h"
// Third-party
#include <SDL3_ttf/SDL_ttf.h>
// Standard
#include <print>
#include <sstream>
#include <debugapi.h>

void Engine::Utils::Check(bool const result, std::string_view const message)
{
    if (!result) ThrowSDLError(message);
}

void Engine::Utils::ThrowSDLError(std::string_view const message)
{
    throw std::runtime_error(std::format("{}: {}", message, SDL_GetError()));
}

void Engine::Utils::LogSDLVersion(std::string_view const message, int const major, int const minor, int const patch)
{
#if WIN32
    std::stringstream ss;
    ss << message << major << "." << minor << "." << patch << "\n";
    OutputDebugString(ss.str().c_str());
#else
    std::println("{} {}.{}.{}", message, major, minor, patch);
#endif
}

void Engine::Utils::PrintSDLVersion()
{
    // Why bother with this? Because sometimes students have a different SDL version installed on their pc.
    // That is not a problem unless for some reason the dll's from this project are not copied next to the exe.
    // These entries in the debug output help to identify that issue.
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

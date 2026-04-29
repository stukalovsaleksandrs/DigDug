#ifndef SE_UTILS_H
#define SE_UTILS_H

// Third-party
#include "SDL3/SDL_error.h"
// Standard
#include <string_view>
#include <format>
#include <stdexcept>

namespace Engine::Utils {
    inline void ThrowSDLError(std::string_view const preceedingMessage) {
        throw std::runtime_error(std::format("{}: {}", preceedingMessage, SDL_GetError()));
    }

    void LogSDLVersion(std::string_view const message, int const major, int const minor, int const patch);

    void PrintSDLVersion();
}

#endif// SE_UTILS_H

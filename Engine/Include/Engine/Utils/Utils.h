#ifndef SE_UTILS_H
#define SE_UTILS_H

// Third-party
#include "SDL3/SDL_error.h"
// Standard
#include <string_view>
#include <format>
#include <stdexcept>

namespace Engine::Utils {
    void Check(bool result, std::string_view message);

    void ThrowSDLError(std::string_view message);

    void LogSDLVersion(std::string_view message, int major, int minor, int patch);

    void PrintSDLVersion();
}

#endif// SE_UTILS_H

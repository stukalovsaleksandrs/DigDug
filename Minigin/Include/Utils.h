#ifndef UTILS_H
#define UTILS_H
#include "SDL3/SDL_error.h"
#include <string_view>
#include <format>
#include <stdexcept>

namespace DAE::Utils {
    inline void ThrowSDLError(std::string_view preceedingMessage) {
        throw std::runtime_error(std::format("{}: {}", preceedingMessage, SDL_GetError()));
    }
}

#endif

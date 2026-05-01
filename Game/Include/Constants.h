#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

// Engine
#include "Engine/Core/Window.h"
// Third-party
#include "glm/vec2.hpp"

namespace Game
{
    glm::uvec2 constexpr characterDims{ 16, 16 };

    Engine::Window::Data const windowData( { 224, 288 }, 3.f );
}

#endif// GAME_CONSTANTS_H
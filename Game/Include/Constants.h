#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

// Engine
#include "Engine/Core/Window.h"
#include "Engine/Core/Observer.h"
// Third-party
#include "glm/vec2.hpp"

namespace Game
{
    glm::uvec2 constexpr characterDims{ 16, 16 };

    Engine::Window::Data const windowData( { 224, 288 }, 3.f );

    namespace EU = Engine::Utils;

    enum class EventType : Engine::EventId
    {
        OnPointsIncreased = EU::MakeSDBMHash("OnPointsIncreased"),
        OnCollected5Points = EU::MakeSDBMHash("OnCollected5Points"),
        OnDied = EU::MakeSDBMHash("OnDied"),
        OnDamageTaken = EU::MakeSDBMHash("OnDamageTaken"),
    };
}

#endif// GAME_CONSTANTS_H

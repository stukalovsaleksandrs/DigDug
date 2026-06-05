#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

// Engine
#include "Engine/Core/Window.hpp"
#include "Engine/Core/Observer.hpp"
#include "Engine/Utils/Utils.hpp"
// Third-party
#include "glm/vec2.hpp"

namespace Game
{
    uint32_t constexpr tileSideLength{ 16 };// in px

    Engine::Window::Data const windowData( { 224, 288 }, 3.f );

    namespace EU = Engine::Utils;

    enum class EventType : Engine::EventId
    {
        OnPointsIncreased = "OnPointsIncreased"_h,
        OnCollected5Points = "OnCollected5Points"_h,
        OnDied = "OnDied"_h,
        OnDamageTaken = "OnDamageTaken"_h,
    };
}

#endif// GAME_CONSTANTS_H

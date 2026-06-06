#ifndef GAME_UTILS
#define GAME_UTILS

// Engine
#include "Engine/Core/Window.hpp"
#include "Engine/Core/Observer.hpp"
#include "Engine/Utils/Utils.hpp"
#include "Engine/Sound/ISoundService.hpp"
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

    enum class SoundName
    {
        GameStart
    };

    void LoadSound(SoundName name, std::string_view path) noexcept;
    void PlaySound(SoundName name, bool looping = false) noexcept;
}

#endif// GAME_UTILS

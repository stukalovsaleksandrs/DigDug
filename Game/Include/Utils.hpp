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

    glm::vec2 constexpr topLeftToCenterOffset{ 0.5f * glm::vec2{ tileSideLength, tileSideLength } };

    namespace EU = Engine::Utils;

    enum class EventType : Engine::EventId
    {
        OnPointsIncreased = "OnPointsIncreased"_h,
        OnCollected5Points = "OnCollected5Points"_h,
        OnDied = "OnDied"_h,
        OnDamageTaken = "OnDamageTaken"_h,
        OnGridChanged = "OnGridChanged"_h,
        OnAttack = "OnAttacked"_h,
    };

    class Subject : public Engine::Subject
    {
    public:
        void NotifyObservers(EventType const type) const noexcept
        {
            Engine::Subject::NotifyObservers(Engine::Event{ std::to_underlying(type) });
        }
    };

    enum class GameAction : uint8_t
    {
        Attack
    };

    enum class SoundName
    {
        GameStart
    };

    void LoadSound(SoundName name, std::string_view path) noexcept;
    void PlaySound(SoundName name, bool looping = false) noexcept;
}

#endif// GAME_UTILS

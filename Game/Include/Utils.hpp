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
    using Cell = glm::i32vec2;
    uint32_t constexpr i32tileSideLengthPx{ 16 };// in px
    float constexpr ftileSideLengthPx{ static_cast<float>(i32tileSideLengthPx) };

    Engine::Window::Data const windowData( { 224, 288 }, 3.f );

    glm::vec2 constexpr topLeftToCenterOffset{ 0.5f * glm::vec2{ ftileSideLengthPx, ftileSideLengthPx } };

    namespace EU = Engine::Utils;

    enum class EventType : Engine::EventId
    {
        OnPointsIncreased = "OnPointsIncreased"_h,
        OnCollected5Points = "OnCollected5Points"_h,
        OnGameOver = "OnDied"_h,
        OnDamageTaken = "OnDamageTaken"_h,
        OnGridChanged = "OnGridChanged"_h,
        OnThrow = "OnAttack"_h,
        OnEnemyDied = "OnEnemyDied"_h,
        OnCaught = "OnCaught"_h,
    };

    class Subject : public Engine::Subject
    {
    public:
        void NotifyObservers(EventType const type) const noexcept
        {
            Engine::Subject::NotifyObservers(Engine::Event{ std::to_underlying(type) });
        }
    };

    enum class SoundName
    {
        GameStart
    };

    void LoadSound(SoundName name, std::string_view path) noexcept;
    void PlaySound(SoundName name, bool looping = false) noexcept;
}

#endif// GAME_UTILS

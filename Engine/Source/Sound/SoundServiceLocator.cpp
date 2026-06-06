#include "Sound/SoundServiceLocator.hpp"

////////////////////////////
/// Null sound service
////////////////////////////

namespace Engine
{
    class NullSoundService final : public ISoundService
    {
    public:
        [[nodiscard]] SoundId LoadSound(std::string_view) noexcept override{ return 0; }
        void PlaySound([[maybe_unused]] const SoundData&) noexcept override{}
        void StopAllSounds() noexcept override{}
        void SetVolume([[maybe_unused]] float volume) noexcept override{}
    };

    static NullSoundService g_nullSoundService;
}

////////////////////////////
/// Sound service locator
////////////////////////////

static Engine::ISoundService* currentSoundService{ &Engine::g_nullSoundService };

Engine::ISoundService& Engine::SoundServiceLocator::GetSoundService() noexcept
{
    return *currentSoundService;
}

void Engine::SoundServiceLocator::SetSoundService(ISoundService& service) noexcept
{
    currentSoundService = &service;
}

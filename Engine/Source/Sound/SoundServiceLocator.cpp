#include "Sound/SoundServiceLocator.h"

////////////////////////////
/// Null sound service
////////////////////////////

namespace Engine
{
    class NullSoundService final : public ISoundService
    {
    public:
        void PlaySound([[maybe_unused]] uint32_t soundId) noexcept override{}
        void StopSound([[maybe_unused]] uint32_t soundId) noexcept override{};
        void StopAllSounds() noexcept override{};
        void SetVolume([[maybe_unused]] uint32_t volume) noexcept override{};
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

void Engine::SoundServiceLocator::RegisterSoundService(ISoundService& service) noexcept
{
    currentSoundService = &service;
}

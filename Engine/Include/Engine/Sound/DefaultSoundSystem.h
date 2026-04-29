#ifndef ENGINE_DEFAULT_SOUND_SERVICE_H
#define ENGINE_DEFAULT_SOUND_SERVICE_H

// Engine
#include "ISoundService.h"
// Standard
#include <memory>

namespace Engine
{
    class DefaultSoundSystem final : public ISoundService
    {
    public:
        DefaultSoundSystem() noexcept;
        ~DefaultSoundSystem() noexcept override;
        DefaultSoundSystem(DefaultSoundSystem const&) noexcept = delete;
        DefaultSoundSystem(DefaultSoundSystem&&) noexcept = delete;
        DefaultSoundSystem operator=(DefaultSoundSystem&&) noexcept = delete;
        DefaultSoundSystem operator=(DefaultSoundSystem const&) noexcept = delete;
        void PlaySound(uint32_t soundId) noexcept override;
        void StopSound(uint32_t soundId) noexcept override;
        void StopAllSounds() noexcept override;
        void SetVolume(uint32_t volume) noexcept override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl{};

    };
}

#endif// ENGINE_DEFAULT_SOUND_SERVICE_H

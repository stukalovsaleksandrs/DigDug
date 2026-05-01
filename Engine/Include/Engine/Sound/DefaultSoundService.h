#ifndef ENGINE_DEFAULT_SOUND_SERVICE_H
#define ENGINE_DEFAULT_SOUND_SERVICE_H

// Engine
#include "ISoundService.h"
// Standard
#include <memory>
#include <string_view>

namespace Engine
{
    class DefaultSoundService final : public ISoundService
    {
    public:
        DefaultSoundService() noexcept;
        ~DefaultSoundService() noexcept override;
        DefaultSoundService(DefaultSoundService const&) noexcept = delete;
        DefaultSoundService(DefaultSoundService&&) noexcept = delete;
        DefaultSoundService operator=(DefaultSoundService&&) noexcept = delete;
        DefaultSoundService operator=(DefaultSoundService const&) noexcept = delete;

        [[nodiscard]] SoundId LoadSound(std::string_view path) override;
        void PlaySound(SoundId soundId) noexcept override;
        void StopAllSounds() noexcept override;
        void SetVolume(float volume) noexcept override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;

    };

}

#endif// ENGINE_DEFAULT_SOUND_SERVICE_H

#ifndef ENGINE_LOGGING_SOUND_SERVICE_H
#define ENGINE_LOGGING_SOUND_SERVICE_H

// Engine
#include "DefaultSoundService.h"

namespace Engine
{
    class LoggingSoundService final : public ISoundService
    {
    public:
        LoggingSoundService(DefaultSoundService&);
        [[nodiscard]] SoundId LoadSound(std::string_view path) override;
        void PlaySound(SoundId soundId) noexcept override;
        void StopAllSounds() noexcept override;
        void SetVolume(float volume) noexcept override;

    private:
        DefaultSoundService& m_defaultSoundService;

    };
}

#endif// ENGINE_LOGGING_SOUND_SERVICE_H

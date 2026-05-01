#ifndef ENGINE_LOGGING_SOUND_SERVICE_H
#define ENGINE_LOGGING_SOUND_SERVICE_H

// Engine
#include "ISoundService.h"

namespace Engine
{
    class LoggingSoundService final : public ISoundService
    {
    public:
        explicit LoggingSoundService(ISoundService&);
        ~LoggingSoundService() override;
        [[nodiscard]] SoundId LoadSound(std::string_view path) override;
        void PlaySound(SoundId soundId) noexcept override;
        void StopAllSounds() noexcept override;
        void SetVolume(float volume) noexcept override;

    private:
        ISoundService& m_realSoundService;

    };
}

#endif// ENGINE_LOGGING_SOUND_SERVICE_H

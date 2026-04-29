#ifndef ENGINE_ISOUND_SERVICE_H
#define ENGINE_ISOUND_SERVICE_H

// Standard
#include <cstdint>

namespace Engine
{
    // Interface for an object providing ability to manipulate sounds
    class ISoundService
    {
    public:
        virtual ~ISoundService() = default;
        virtual void PlaySound(uint32_t soundId) noexcept = 0;
        virtual void StopSound(uint32_t soundId) noexcept = 0;
        virtual void StopAllSounds() noexcept = 0;
        virtual void SetVolume(uint32_t volume) noexcept = 0;

    };
}

#endif// ENGINE_ISOUND_SERVICE_H

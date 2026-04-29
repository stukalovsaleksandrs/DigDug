#ifndef ENGINE_ISOUND_SERVICE_H
#define ENGINE_ISOUND_SERVICE_H

// Standard
#include <cstdint>
#include <string_view>

namespace Engine
{
    // Opaque handle for clients to be able to refer to a specific loaded sound
    using SoundId = uint32_t;

    // Interface for an object providing ability to manipulate sounds
    class ISoundService
    {
    public:
        virtual ~ISoundService() = default;
        [[nodiscard]] virtual SoundId LoadSound(std::string_view path) = 0;
        virtual void PlaySound(SoundId soundId) noexcept = 0;
        virtual void StopAllSounds() noexcept = 0;
        virtual void SetVolume(SoundId volume) noexcept = 0;

    };
}

#endif// ENGINE_ISOUND_SERVICE_H

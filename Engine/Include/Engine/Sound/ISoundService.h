#ifndef ENGINE_ISOUND_SERVICE_H
#define ENGINE_ISOUND_SERVICE_H

#include "glm/fwd.hpp"
// Standard
#include <cstdint>
#include <string_view>

namespace Engine
{
    // Opaque handle for clients to be able to refer to a specific loaded sound
    using SoundId = glm::uint32_t;

    // Interface for an object providing ability to manipulate sounds
    class ISoundService
    {
    public:
        virtual ~ISoundService() = default;
        [[nodiscard]] virtual SoundId LoadSound(std::string_view path) = 0;
        virtual void PlaySound(SoundId soundId) noexcept = 0;
        virtual void StopAllSounds() noexcept = 0;
        virtual void SetVolume(float volume) noexcept = 0;

    };
}

#endif// ENGINE_ISOUND_SERVICE_H

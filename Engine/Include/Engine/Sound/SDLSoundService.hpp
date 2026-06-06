#ifndef ENGINE_SDL_SOUND_SERVICE
#define ENGINE_SDL_SOUND_SERVICE

// Engine
#include "ISoundService.hpp"
// Standard
#include <memory>
#include <string_view>
#include <format>// Required by the Emscripten

namespace Engine
{
    class SDLSoundService final : public ISoundService
    {
    public:
        SDLSoundService() noexcept;
        ~SDLSoundService() noexcept override;
        SDLSoundService(SDLSoundService const&) noexcept = delete;
        SDLSoundService(SDLSoundService&&) noexcept = delete;
        SDLSoundService& operator=(SDLSoundService&&) noexcept = delete;
        SDLSoundService& operator=(SDLSoundService const&) noexcept = delete;

        [[nodiscard]] SoundId LoadSound(std::string_view path) override;
        void PlaySound(SoundData const&) noexcept override;
        void StopAllSounds() noexcept override;
        void SetVolume(float volume) noexcept override;

    private:
        class Impl;
        std::unique_ptr<Impl> m_pImpl;
    };

}

#endif// ENGINE_SDL_SOUND_SERVICE

// Engine
#include "Utils/Utils.hpp"
#include "Sound/SDLSoundService.hpp"
// Third-party
#include "SDL3/SDL.h"
#include "SDL3_mixer/SDL_mixer.h"
// Standard
#include <functional>
#include <thread>

////////////////////////////
/// DefaultSoundSystem::Impl
////////////////////////////

class Engine::SDLSoundService::Impl final
{
public:
    Impl()
    {
        // 1. Initializing SDL_mixer
        Utils::Check(MIX_Init(), "MIX_Init failed");

        // 2. Creating a mixer instance
        m_pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        Utils::Check(m_pMixer,"Mixer creation failed");

        // 3. Creating a sound effect track
        m_pTrack = MIX_CreateTrack(m_pMixer);
        Utils::Check(m_pTrack,"Sound effect track creation failed");

        // Starting a separate thread for sound
        // m_soundThread = std::jthread([this]()
        // {
        //     while ()
        // });
    }

    ~Impl()
    {
        // Destroying audio objects
        for (auto* pAudio : m_pAudio)
        {
            MIX_DestroyAudio(pAudio);
        }

        // Stopping playback globally
        MIX_StopTrack(m_pTrack, 0);

        // Destroying track first
        MIX_DestroyTrack(m_pTrack);

        // Destroying mixer
        MIX_DestroyMixer(m_pMixer);

        // Shutting the service down
        MIX_Quit();
    }
    Impl(Impl&&) noexcept = delete;
    Impl& operator=(Impl&&) noexcept = delete;
    Impl(Impl const&) noexcept = delete;
    Impl& operator=(Impl const&) noexcept = delete;

    [[nodiscard]] SoundId LoadSound(std::string_view const path)
    {
        MIX_Audio* const pAudio{ MIX_LoadAudio(m_pMixer, path.data(), true) };
        Utils::Check(pAudio, std::format("Failed to load {}", path));
        m_pAudio.push_back(pAudio);
        return static_cast<SoundId>(m_pAudio.size() - 1);
    }

    void PlaySound(SoundId const soundHandle) const noexcept
    {
        MIX_SetTrackAudio(m_pTrack, m_pAudio.at(soundHandle));
        MIX_PlayTrack(m_pTrack, 0);
    }

    void StopAllSounds() const noexcept
    {
        MIX_StopTrack(m_pTrack, 10);
    }

    void SetVolume(float const volume) const noexcept
    {
        MIX_SetMixerGain(m_pMixer, volume);
    }

private:
    MIX_Mixer* m_pMixer{};
    MIX_Track* m_pTrack{};
    std::vector<MIX_Audio*> m_pAudio{};
    std::jthread m_soundThread{};

};

////////////////////////////
/// DefaultSoundSystem
////////////////////////////

Engine::SDLSoundService::SDLSoundService() noexcept
    : m_pImpl{ std::make_unique<Impl>() }{}

// Destructor requires the implementation class to be defined, see:
// https://stackoverflow.com/questions/34072862/why-is-error-invalid-application-of-sizeof-to-an-incomplete-type-using-uniqu/34073093
// https://cpppatterns.com/patterns/pimpl.html
Engine::SDLSoundService::~SDLSoundService() noexcept = default;

Engine::SoundId Engine::SDLSoundService::LoadSound(std::string_view const path)
{
    return m_pImpl->LoadSound(path);
}

void Engine::SDLSoundService::PlaySound(glm::uint32_t const soundId) noexcept
{
    m_pImpl->PlaySound(soundId);
}

void Engine::SDLSoundService::StopAllSounds() noexcept
{
    m_pImpl->StopAllSounds();
}

void Engine::SDLSoundService::SetVolume(float const volume) noexcept
{
    m_pImpl->SetVolume(volume);
}


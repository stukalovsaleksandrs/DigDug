// Engine
#include "Utils/Utils.h"
#include "Sound/DefaultSoundSystem.h"
// Third-party
#include "SDL3/SDL.h"
#include "SDL3_mixer/SDL_mixer.h"

////////////////////////////
/// DefaultSoundSystem::Impl
////////////////////////////

class Engine::DefaultSoundSystem::Impl
{
public:
    Impl()
    {
        // Initializing SDL_mixer
        Utils::Check(MIX_Init(), "MIX_Init failed");

        m_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        Utils::Check(m_mixer,"Mixer creation failed");
    }



private:
    MIX_Mixer *m_mixer{};
    // MIX_Track *track{};
    // MIX_Audio *audio{};
};

////////////////////////////
/// DefaultSoundSystem
////////////////////////////

Engine::DefaultSoundSystem::DefaultSoundSystem() noexcept
    : m_pImpl{ std::make_unique<Impl>() }
{}

// Destructor requires the implementation class to be defined, see:
// https://stackoverflow.com/questions/34072862/why-is-error-invalid-application-of-sizeof-to-an-incomplete-type-using-uniqu/34073093
// https://cpppatterns.com/patterns/pimpl.html
Engine::DefaultSoundSystem::~DefaultSoundSystem() noexcept = default;

void Engine::DefaultSoundSystem::PlaySound([[maybe_unused]] uint32_t soundId) noexcept
{
    // m_pImpl->PlaySound();
}

void Engine::DefaultSoundSystem::StopSound([[maybe_unused]] uint32_t soundId) noexcept
{
    // m_pImpl->Sound();
}

void Engine::DefaultSoundSystem::StopAllSounds() noexcept
{
}

void Engine::DefaultSoundSystem::SetVolume([[maybe_unused]] uint32_t volume) noexcept
{
}


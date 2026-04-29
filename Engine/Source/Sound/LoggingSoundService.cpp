// Engine
#include "Engine/Sound/LoggingSoundService.h"
// Standard
#include <print>

Engine::SoundId Engine::LoggingSoundService::LoadSound(std::string_view const path)
{
    std::println("Loading sound: {}", path);
    return m_defaultSoundService.LoadSound(path);
}

void Engine::LoggingSoundService::PlaySound(SoundId const soundId) noexcept
{
    std::println("Playing sound at ID {}", soundId);
    m_defaultSoundService.PlaySound(soundId);
}

void Engine::LoggingSoundService::StopAllSounds() noexcept
{
    std::println("Stopping all sounds");
    m_defaultSoundService.StopAllSounds();
}

void Engine::LoggingSoundService::SetVolume(float const volume) noexcept
{
    std::println("Volume: {}", volume);
    m_defaultSoundService.SetVolume(volume);
}

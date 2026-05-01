// Engine
#include "Engine/Sound/LoggingSoundService.h"
// Standard
#include <print>

Engine::LoggingSoundService::LoggingSoundService(ISoundService& defaultSoundService)
    : m_realSoundService{ defaultSoundService }
{}

Engine::LoggingSoundService::~LoggingSoundService() = default;

Engine::SoundId Engine::LoggingSoundService::LoadSound(std::string_view const path)
{
    std::println("Loading sound: {}", path);
    return m_realSoundService.LoadSound(path);
}

void Engine::LoggingSoundService::PlaySound(SoundId const soundId) noexcept
{
    std::println("Playing sound at ID {}", soundId);
    m_realSoundService.PlaySound(soundId);
}

void Engine::LoggingSoundService::StopAllSounds() noexcept
{
    std::println("Stopping all sounds");
    m_realSoundService.StopAllSounds();
}

void Engine::LoggingSoundService::SetVolume(float const volume) noexcept
{
    std::println("Volume: {}", volume);
    m_realSoundService.SetVolume(volume);
}

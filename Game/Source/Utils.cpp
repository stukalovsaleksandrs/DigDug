// Game
#include "Utils.hpp"
// Engine
#include "Engine/Sound/SoundServiceLocator.hpp"

static std::unordered_map<Game::SoundName, Engine::SoundId> g_soundNameToId;

void Game::LoadSound(SoundName const name, std::string_view const path) noexcept
{
    static auto& soundService{ Engine::SoundServiceLocator::GetSoundService() };
    Engine::SoundId const id{ soundService.LoadSound(path) };
    g_soundNameToId.insert({name, id});
}

void Game::PlaySound(SoundName const name, bool const looping) noexcept
{
    static auto& soundService{ Engine::SoundServiceLocator::GetSoundService() };
    soundService.PlaySound({g_soundNameToId.at(name), looping});
}

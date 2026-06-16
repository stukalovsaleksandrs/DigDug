// Game
#include "Utils.hpp"
#include "Application.hpp"
// Engine
#include "Engine/Sound/SoundServiceLocator.hpp"
#include "Engine/Rendering/Sprite.hpp"
// Standard
#include <filesystem>

namespace fs = std::filesystem;

[[nodiscard]] std::string GetResourceFolderPath()
{
#if __EMSCRIPTEN__
    fs::path resourceFolderLocation = "";
#else
    auto resourceFolderName{"Resources"};
    fs::path resourceFolderLocation = std::format("./{}/", resourceFolderName);
    if (!fs::exists(resourceFolderLocation))
        resourceFolderLocation = std::format("../{}/", resourceFolderName);
#endif

    return resourceFolderLocation.generic_string();
}

Game::Application::Application()
    : Engine::Application(windowData, "Dig Dug")
{
    // cd into the resource directory
    fs::current_path(GetResourceFolderPath());

    // Playing sound
    auto& soundService{ Engine::SoundServiceLocator::GetSoundService() };
    soundService.SetVolume(0.05f);
    LoadSound(SoundName::GameStart, "GameStart.mp3");
    PlaySound(SoundName::GameStart);

    // Levels
    // Don't move to the header, has to be added to rendering after the ground
    // TODO: Actually move to header
    m_pLevelManager = std::make_unique<LevelManager>(std::vector<std::string_view>{"Levels/Level1.txt"});
}

void Game::Application::Update()
{
    m_pLevelManager->Update();
}

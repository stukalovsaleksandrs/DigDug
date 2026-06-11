// Game
#include "Levels/LevelManager.hpp"
// Engine
#include "Engine/Rendering/Font.hpp"
// Standard
#include <memory>

Game::LevelManager::LevelManager(std::vector<std::string_view> const& paths)
    : m_paths{ paths }// Parsed lazily on load
    // Resources get reused in levels, so they are preloaded at startup
    , m_resources{
        .pFont = std::make_unique<Engine::Font>("Lingua.otf", 36),
        .pTaizoHoriSprite = std::make_unique<Engine::Sprite>("Sprites/TaizoHori/Default.png"),
        .pPookaSprite = std::make_unique<Engine::Sprite>("Sprites/Pooka/Default.png"),
        .pGroundSprite = std::make_unique<Engine::Sprite>("Sprites/Background/Ground.png"),
        .pSkySprite =  std::make_unique<Engine::Sprite>("Sprites/Background/Sky.png"),
        .pPumpSprite = std::make_unique<Engine::Sprite>("Sprites/TaizoHori/Pump.png")
    }
{}

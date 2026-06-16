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
        .pTaizoHoriDefaultSprite = std::make_unique<Engine::Sprite>("Sprites/TaizoHori/Default.png"),
        .pPookaDefaultSprite = std::make_unique<Engine::Sprite>("Sprites/Pooka/Default.png"),
        .pPookaPumpedSprite = std::make_unique<Engine::Sprite>("Sprites/Pooka/Pumped.png"),
        .pGroundSprite = std::make_unique<Engine::Sprite>("Sprites/Background/Ground.png"),
        .pSkySprite =  std::make_unique<Engine::Sprite>("Sprites/Background/Sky.png"),
        .pPumpSprite = std::make_unique<Engine::Sprite>("Sprites/TaizoHori/Pump.png"),
    }
{}

void Game::LevelManager::Update() noexcept
{
    if (m_shouldRestart)
    {
        m_pCurrentLevel.reset();
        Engine::Renderer::GetInstance().ClearRenderFunctions();
        m_pCurrentLevel = std::make_unique<Level>(m_paths.at(m_currentLevelIdx), m_resources);
        m_pCurrentLevel->BindObserver(*this);
        m_shouldRestart = false;
    }

    m_pCurrentLevel->Update();
}

void Game::LevelManager::OnNotify(Engine::Event const event, Engine::Subject const&) noexcept
{
    switch (event.id)
    {
    case std::to_underlying(EventType::OnGameOver):
        m_shouldRestart = true;
        break;
    default: ;
    }
}

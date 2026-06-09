// Game
#include "Levels/LevelManager.hpp"

Game::LevelManager::LevelManager(std::vector<std::string_view> const& paths)
{
    // Paring the files
    m_levels.reserve(paths.size());
    for (auto& path : paths)
        m_levels.emplace_back(std::make_unique<Level>(path));

    // Subscribing the first level's rendering function to the renderer
    if (not m_levels.empty())
         m_levels.front()->SetActive(true);
}

#ifndef GAME_LEVEL_MANAGER
#define GAME_LEVEL_MANAGER

// Game
#include "Level.hpp"
// Standard
#include <memory>
#include <string_view>
#include <vector>

namespace Game
{
    class Level;

    class LevelManager
    {
    public:
        explicit LevelManager(std::vector<std::string_view> const& paths);
        [[nodiscard]] Level& GetCurrentLevel() const noexcept{ return *m_pCurrentLevel; };

    private:
        std::vector<std::string_view> const m_paths;
        Level::Resources m_resources;
        uint32_t m_currentLevelIdx{};
        // Loading the first level
        std::unique_ptr<Level> m_pCurrentLevel{ std::make_unique<Level>(m_paths.at(m_currentLevelIdx), m_resources) };
    };

}

#endif// GAME_LEVEL_MANAGER

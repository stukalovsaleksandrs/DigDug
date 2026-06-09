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
    class LevelManager
    {
    public:
        explicit LevelManager(std::vector<std::string_view> const& paths);
        [[nodiscard]] Level& GetCurrentLevel() const noexcept{ return *m_levels.at(m_currentLevelIdx); };

    private:
        std::vector<std::unique_ptr<Level>> m_levels;
        uint32_t m_currentLevelIdx{};

    };

}

#endif// GAME_LEVEL_MANAGER

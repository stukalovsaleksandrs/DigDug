#ifndef LEVEL_PARSER
#define LEVEL_PARSER
// Game
#include "Grid.hpp"
// Standard
#include <memory>
#include <string_view>
#include <vector>

namespace Game
{
    struct Level final
    {
        glm::u32vec2 characterSpawnCell{};
        std::vector<glm::u32vec2> pookaSpawnCells{},
            flygarSpawnCells{}, rockSpawnCells{};
        Grid grid;
    };
    class Levels final
    {
    public:
        explicit Levels(std::vector<std::string_view> const& paths);
        [[nodiscard]] Level& GetCurrentLevel() noexcept{ return *m_levels.at(m_currentLevelIdx); };

    private:
        std::vector<std::unique_ptr<Level>> m_levels;
        uint32_t m_currentLevelIdx{};

        [[nodiscard]] std::unique_ptr<Level> ParseFile(std::string_view path);
        static void ParseCharacter(Level&, std::string_view path, std::string_view line, glm::u32vec2 cell);
    };
}

#endif// LEVEL_PARSER

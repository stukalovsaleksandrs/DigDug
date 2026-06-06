// Game
#include "Levels.hpp"
// Standard
#include <algorithm>
#include <format>
#include <fstream>

Game::Levels::Levels(std::vector<std::string_view> const& paths)
{
    m_levels.reserve(paths.size());
    std::ranges::transform(paths, std::back_inserter(m_levels), [this](std::string_view const path)
    {
        return ParseFile(path);
    });
}

[[nodiscard]] std::string_view GetExtension(std::string_view const path)
{
    return path.substr(path.find_last_of('.') + 1);
}

void ValidateExtension(std::string_view const path, std::string_view const extension)
{
    if (!path.ends_with(extension)) {
        throw std::runtime_error(
            std::format("Level file extension is incorrect: got {}, want {}", GetExtension(path), extension)
        );
    }
}

std::unique_ptr<Game::Level> Game::Levels::ParseFile(std::string_view const path)
{
    // Validating file extension
    ValidateExtension(path, ".txt");

    // Open the file, checking if exists
    std::ifstream file{ path.data() };
    if (not file.is_open())
        throw std::runtime_error{ std::format("Failed to open level file: {}", path) };

    // Parsing the file
    auto pLevel{ std::make_unique<Level>() };

    std::string line{};
    uint32_t row{};
    while (std::getline(file, line))
    {
        for (uint32_t const col : std::ranges::views::iota(0u, line.size()))
        {
            glm::u32vec2 const cell{ col, row };
            ParseCharacter(*pLevel, path, line, cell);
        }
        ++row;
    }

    return pLevel;
}

void Game::Levels::ParseCharacter(Level& level, std::string_view const path, std::string_view const line, glm::u32vec2 const cell)
{
    switch (line.at(cell.x))
    {
    case '1':
    {
        if (level.characterSpawnCell != glm::u32vec2{})
            throw std::runtime_error{ std::format("{} has multiple character spawn points", path.data()) };

        level.characterSpawnCell = cell;
        break;
    }
    case '2': level.pookaSpawnCells.push_back(cell); break;
    case '3': level.flygarSpawnCells.push_back(cell); break;
    case '*': level.rockSpawnCells.push_back(cell); break;
    case '.': /* grid is all ground by default */ break;
    case ' ': level.grid.SetAir(cell); break;
    default:
        throw std::runtime_error(std::format(
            "Unknown character '{}' at ({}, {}) in {}", line.at(cell.x), cell.x, cell.y, path
        ));
    }
}

// Game
#include "Levels/Level.hpp"
// Engine
#include "Engine/Rendering/Renderer.hpp"
// Standard
#include <format>
#include <fstream>

Game::Level::Level(std::string_view const path) noexcept
    : m_maskTexture{ SDL_Color{255, 255, 255, 255}, windowData.logicalDims, SDL_BLENDMODE_MOD}
{
    ParseFile(path);
}

void Game::Level::SetActive(bool const active) const noexcept
{
    if (active)
    {
        // Registering the tunnel digging render callback to the renderer
        Engine::Renderer::GetInstance().RegisterFunction(m_renderTunnelsFunction, Engine::Renderer::Layer::background);
    }
    else
    {
        Engine::Renderer::GetInstance().UnregisterFunction(m_renderTunnelsFunction);
    }
}

void Game::Level::Dig(glm::vec2 const px) const noexcept
{
    static float constexpr halfTileSideLength{ 0.5f * tileSideLength };
    m_maskTexture.MaskCircle({px, halfTileSideLength});
}

glm::vec2 Game::Level::GetCellTopLeft(glm::u32vec2 const px) const noexcept
{
    glm::i32vec2 const cell{ m_grid.GetCellFromPoint(px) };
    return m_grid.GetCellTopLeft(cell);
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

void Game::Level::ParseFile(std::string_view const path)
{
    // Validating file extension
    ValidateExtension(path, ".txt");

    // Open the file, checking if exists
    std::ifstream file{ path.data() };
    if (not file.is_open())
        throw std::runtime_error{ std::format("Failed to open level file: {}", path) };

    std::string line{};
    uint32_t row{};
    while (std::getline(file, line))
    {
        for (uint32_t const col : std::ranges::views::iota(0u, line.size()))
        {
            glm::u32vec2 const cell{ col, row };
            ParseCharacter(path, line, cell);
        }
        ++row;
    }

    // Punching the holes in the ground
    CreateInitialTunnels();
}

void Game::Level::ParseCharacter(std::string_view const path, std::string_view const line, glm::u32vec2 const cell)
{
    switch (line.at(cell.x))
    {
    case '1':
    {
        if (m_characterSpawnCell != glm::u32vec2{})
            throw std::runtime_error{ std::format("{} has multiple character spawn points", path.data()) };

        m_characterSpawnCell = cell;
        break;
    }
    case '2': m_pookaSpawnCells.push_back(cell); break;
    case '3': m_flygarSpawnCells.push_back(cell); break;
    case '*': m_rockSpawnCells.push_back(cell); break;
    case '.': /* grid is all ground by default */ break;
    case ' ': m_grid.SetAir(cell); break;
    default:
        throw std::runtime_error(std::format(
            "Unknown character '{}' at ({}, {}) in {}", line.at(cell.x), cell.x, cell.y, path
        ));
    }
}

void Game::Level::CreateInitialTunnels() noexcept
{

}

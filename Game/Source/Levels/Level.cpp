// Game
#include "Levels/Level.hpp"
// Engine
#include "Engine/Rendering/Renderer.hpp"
// Standard
#include <format>
#include <fstream>

Game::Level::Level(std::string_view const path) noexcept
    : m_maskTexture{ SDL_Color{255, 255, 255, 255}, windowData.logicalDims, SDL_BLENDMODE_MOD}
    , m_charToParsingFunc{
        { '1', [this](glm::i32vec2 const cell)
        {
            if (m_characterSpawnCell != glm::u32vec2{})
                throw std::runtime_error{ std::format("A file has multiple character spawn points") };

            m_characterSpawnCell = cell;
            m_grid.SetAir(cell);
        }},
        { '2', [this](glm::i32vec2 const cell)
        {
            m_pookaSpawnCells.push_back(cell);
            m_grid.SetAir(cell);
        }},
        {'3', [this](glm::i32vec2 const cell)
        {
            m_flygarSpawnCells.push_back(cell);
            m_grid.SetAir(cell);
        }},
        {'*', [this](glm::i32vec2 const cell)
        {
            m_rockSpawnCells.push_back(cell);
        }},
        {'.', [](glm::i32vec2 const)
        {
            // grid is all ground by default
        }},
        {' ', [this](glm::i32vec2 const cell)
        {
            m_grid.SetAir(cell);
        }}
    }
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

void Game::Level::DigCircle(glm::vec2 const centerPx) const noexcept
{
    static float constexpr halfTileSideLength{ 0.5f * tileSideLength };
    m_maskTexture.MaskCircle({centerPx, halfTileSideLength});
}

glm::vec2 Game::Level::GetCellTopLeft(glm::u32vec2 const px) const noexcept
{
    glm::i32vec2 const cell{ m_grid.PointToCell(px) };
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
            ParseCharacter(line, cell);
        }
        ++row;
    }

    MaskInitialTunnels();
}

void Game::Level::ParseCharacter(std::string_view const line, glm::u32vec2 const cell) const
{
    char const c{ line.at(cell.x) };
    m_charToParsingFunc.at(c)(cell);
}

void Game::Level::DigSquare(glm::vec2 const topLeftPx, EU::Square::Corners const corners) const noexcept
{
    m_maskTexture.MaskSquare({
        topLeftPx,
        static_cast<float>(tileSideLength),
        corners
    });
}

void Game::Level::MaskInitialTunnels() const noexcept
{
    glm::i32vec2 const dims{
        static_cast<int32_t>(windowData.logicalDims.x) / tileSideLength,
        static_cast<int32_t>(windowData.logicalDims.y) / tileSideLength
    };

    for (int32_t row{}; row < dims.y; ++row)
    {
        for (int32_t col{}; col < dims.x; ++col)
        {
            glm::i32vec2 const cell{ col, row };
            if (m_grid.IsGround(cell)) continue;

            // Check the four cardinal neighbors
            bool const airLeft  { !m_grid.IsGround({ col - 1, row }) };
            bool const airRight { !m_grid.IsGround({ col + 1, row }) };
            bool const airUp    { !m_grid.IsGround({ col, row - 1 }) };
            bool const airDown  { !m_grid.IsGround({ col, row + 1 }) };

            // A corner is sharp if ANY adjacent neighbor in that corner's directions is air.
            // e.g. top-left corner is sharp if the cell to the left OR above is air.
            EU::Square::Corners const corners
            {
                .topLeft     = !airLeft  && !airUp,
                .topRight    = !airRight && !airUp,
                .bottomLeft  = !airLeft  && !airDown,
                .bottomRight = !airRight && !airDown,
            };

            DigSquare(Grid::GetCellTopLeft(cell), corners);
        }
    }
}

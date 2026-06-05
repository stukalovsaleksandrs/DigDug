//#define ENABLE_DEBUG_DRAWING

// Game
#include "Constants.hpp"
#include "Grid.hpp"
// Engine
#include "Engine/Rendering/Renderer.hpp"
// Standard
#include <ranges>

Game::Grid::Grid(int32_t const cellSideLength, glm::ivec2 const logicalWindowDims)
    : m_cellSideLength{ cellSideLength }
    , m_dimsInPx{ logicalWindowDims }
    , m_dimsInCells{ m_dimsInPx.x / m_cellSideLength, m_dimsInPx.y / m_cellSideLength }
    , m_renderer{ Engine::Renderer::GetInstance() }
    , m_isGround( m_dimsInCells.x * m_dimsInCells.y )
{
    // Validating input
    assert(
        m_dimsInPx.x % m_cellSideLength == 0
        && m_dimsInPx.y % m_cellSideLength == 0
        && "Window dimensions are not divisible by the cell side length"
    );

    // Registering rendering callback
    m_renderer.RegisterFunction(m_renderFunction);

    // Filling marking the ground tiles with true
    uint32_t constexpr upperAirRowCount{ 2 };
    std::fill(m_isGround.begin() + m_dimsInCells.x * upperAirRowCount, m_isGround.end(), true);
}

Game::Grid::~Grid() noexcept
{
    m_renderer.UnregisterFunction(m_renderFunction);
}

void Game::Grid::Render() const
{
#ifdef ENABLE_DEBUG_DRAWING
    // Colls
    for(auto const collIdx : std::views::iota(1, m_dimsInCells.x))
    {
        m_renderer.RenderLine(
            {m_cellSideLength * collIdx, 0},
            {m_cellSideLength * collIdx, m_dimsInPx.y}
        );
    }

    // Rows
    for(auto const rowIdx : std::views::iota(1, m_dimsInCells.y))
    {
        m_renderer.RenderLine(
            {0, m_cellSideLength * rowIdx,},
            {m_dimsInPx.y, m_cellSideLength * rowIdx}
        );
    }
#endif// ENABLE_DEBUG_DRAWING
}

glm::ivec2 Game::Grid::GetCellFromPoint(glm::vec2 const point) const noexcept
{
    return {
        static_cast<int>(point.x / static_cast<float>(m_cellSideLength)),
        static_cast<int>(point.y / static_cast<float>(m_cellSideLength))
    };
}

glm::vec2 Game::Grid::GetCellTopLeft(glm::ivec2 const cell) const noexcept
{
    return {
        static_cast<float>(cell.x * m_cellSideLength),
        static_cast<float>(cell.y * m_cellSideLength),
    };
}

bool Game::Grid::TryDigging(glm::ivec2 const pointInPx) noexcept
{
    uint32_t const newCellIdx{ pointInPx.y / tileSideLength * m_dimsInCells.x + pointInPx.x / tileSideLength };

    // Updating current cell if changed
    if (newCellIdx != m_currentCellIdx)
    {
        // NOTE: 0 is unreachable(top left)
        if (m_currentCellIdx != 0) m_isGround.at(m_currentCellIdx) = false;
        m_currentCellIdx = newCellIdx;
    }

    return m_isGround.at(newCellIdx);
}

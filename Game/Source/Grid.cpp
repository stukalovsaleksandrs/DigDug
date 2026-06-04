//#define ENABLE_DEBUG_DRAWING

// Game
#include "Grid.h"
// Engine
#include "Engine/Rendering/Renderer.h"
// Standard
#include <ranges>

Game::Grid::Grid(int32_t const cellSideLength, glm::ivec2 const logicalWindowDims)
    : m_cellSideLength{ cellSideLength }
    , m_dimsInPx{ logicalWindowDims }
    , m_renderer{ Engine::Renderer::GetInstance() }
{
    assert(
        m_dimsInPx.x % m_cellSideLength == 0
        && m_dimsInPx.y % m_cellSideLength == 0
        && "Window dimensions are not divisible by the cell side length"
    );

    m_dimsInCells.x = m_dimsInPx.x / m_cellSideLength;
    m_dimsInCells.y = m_dimsInPx.y / m_cellSideLength;

    m_renderer.RegisterFunction(m_renderFunction);
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
        static_cast<int>(point.x / m_cellSideLength),
        static_cast<int>(point.y / m_cellSideLength)
    };
}

glm::vec2 Game::Grid::GetCellTopLeft(glm::ivec2 const cell) const noexcept
{
    return {
        static_cast<float>(cell.x * m_cellSideLength),
        static_cast<float>(cell.y * m_cellSideLength),
    };
}

void Game::Grid::TryDigging(glm::ivec2 const point, int32_t const radius) noexcept
{

}

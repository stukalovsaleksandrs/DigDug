// #define ENABLE_DEBUG_DRAWING

// Game
#include "Utils.hpp"
#include "Grid.hpp"
// Engine
#include "Engine/Rendering/Renderer.hpp"
// Standard
#include <ranges>
#include <print>

Game::Grid::Grid()
    : m_renderer{ Engine::Renderer::GetInstance() }
    , m_isGround( m_dimsInCells.x * m_dimsInCells.y )
{
    // Validating input
    assert(
        m_dimsInPx.x % tileSideLength == 0
        && m_dimsInPx.y % tileSideLength == 0
        && "Window dimensions are not divisible by the cell side length"
    );

    // Filling it all with ground
    std::ranges::fill(m_isGround, true);

    // Registering rendering callback
    m_renderer.RegisterFunction(m_debugRenderFunction);
}

Game::Grid::~Grid() noexcept
{
    m_renderer.UnregisterFunction(m_debugRenderFunction);
}

void Game::Grid::DebugRender() const
{
#ifdef ENABLE_DEBUG_DRAWING
    for (int32_t const row : std::ranges::views::iota(0, m_dimsInCells.y) )
    {
        for (int32_t const coll : std::ranges::views::iota(0, m_dimsInCells.x) )
        {
            glm::i32vec2 const cell{coll, row};
            glm::vec2 const topLeft = GetCellTopLeft(cell);

            // Choose color based on ground/air
            SDL_FColor const color{
                IsGround(cell)
                ? SDL_FColor{139, 69, 19, 255}// Brown
                : SDL_FColor{255, 255, 255, 255}// White
            };

            m_renderer.RenderSquare(
                Engine::Utils::Square{
                    topLeft,
                    tileSideLength
            }, color
            );
        }
    }
#endif// ENABLE_DEBUG_DRAWING
}

glm::i32vec2 Game::Grid::GetCellFromPoint(glm::vec2 const point) const noexcept
{
    return {
        static_cast<int>(point.x / static_cast<float>(tileSideLength)),
        static_cast<int>(point.y / static_cast<float>(tileSideLength))
    };
}

glm::vec2 Game::Grid::GetCellTopLeft(glm::i32vec2 const cell) const noexcept
{
    return {
        static_cast<float>(cell.x * tileSideLength),
        static_cast<float>(cell.y * tileSideLength),
    };
}

glm::vec2 Game::Grid::GetCellTopLeftFromCellCenter(glm::u32vec2 const centerPx) const noexcept
{
    glm::i32vec2 const cell{ GetCellFromPoint(centerPx) };
    return GetCellTopLeft(cell);
}


glm::vec2 Game::Grid::GetCellCenter(glm::i32vec2 const cell) const noexcept
{
    static glm::vec2 constexpr offset{ 0.5f * glm::vec2{tileSideLength, tileSideLength} };
    return GetCellTopLeft(cell) + offset;
}

bool Game::Grid::TryDigging(glm::i32vec2 const pointInPx) noexcept
{
    uint32_t const newCellIdx{ pointInPx.y / tileSideLength * m_dimsInCells.x + pointInPx.x / tileSideLength };

    // Updating current cell if changed
    if (newCellIdx != m_currentCellIdx)
    {
        // NOTE: 0 is unreachable(top left)
        if (m_currentCellIdx != 0)
        {
            m_isGround.at(m_currentCellIdx) = false;
            // Broadcasting an event that a cell was dug
            NotifyObservers(m_gridChangedEvent);
        }
        m_currentCellIdx = newCellIdx;
    }

    return m_isGround.at(newCellIdx);
}

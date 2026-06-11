#ifndef GAME_GRID
#define GAME_GRID

// Game
#include "Utils.hpp"
// Third-party
#include "glm/vec2.hpp"
// Standard
#include <functional>

namespace Engine
{
    class Renderer;
}

namespace Game
{
    class Grid final : public Engine::Subject
    {
    public:
        explicit Grid();
        ~Grid() noexcept override;
        Grid(Grid const&) noexcept = delete;
        Grid(Grid &&) noexcept = delete;
        Grid& operator=(Grid const&) noexcept = delete;
        Grid& operator=(Grid &&) noexcept = delete;

        void Render() const;
        // Returns the location(in cells) of the cell the input point(in pixels) is in
        [[nodiscard]] glm::i32vec2 GetCellFromPoint(glm::vec2 point) const noexcept;

        // Given a px, gets a cell of this px and returns top left px of this cell
        [[nodiscard]] glm::vec2 GetCellTopLeftFromCellCenter(glm::u32vec2 centerPx) const noexcept;

        // Returns the location(in pixels) of the top left corner of the input cell(in cells)
        [[nodiscard]] glm::vec2 GetCellTopLeft(glm::i32vec2 cell) const noexcept;

        [[nodiscard]] glm::vec2 GetCellCenter(glm::i32vec2 cell) const noexcept;

        [[nodiscard]] glm::i32vec2 GetDimsInCells() const noexcept { return m_dimsInCells; }

        // Returns whether the point is in the ground and preserves the current location
        bool TryDigging(glm::i32vec2 pointInPx) noexcept;

        void SetAir(glm::i32vec2 const cell) noexcept{ At(cell) = false; };

        [[nodiscard]] bool IsGround(glm::i32vec2 const cell) const noexcept
        {
            if (cell.x < 0 || cell.y < 0 || cell.x >= m_dimsInCells.x || cell.y >= m_dimsInCells.y)
                return false; // treat out-of-bounds as air so border cells do not get rounded edges
            return At(cell);
        }

    private:
        Engine::Event m_gridChangedEvent{ std::to_underlying(EventType::OnGridChanged) };

        glm::i32vec2 m_dimsInPx{ windowData.logicalDims  };
        glm::i32vec2 m_dimsInCells{ m_dimsInPx.x / tileSideLength, m_dimsInPx.y / tileSideLength };
        Engine::Renderer& m_renderer;
        std::function<void()> m_renderFunction{ [this]{ Render(); } };

        // Array representing 2D grid of bools communicating whether each cell is ground or not
        std::vector<bool> m_isGround{ };
        uint32_t m_currentCellIdx{};

        [[nodiscard]] std::vector<bool>::reference At(glm::i32vec2 const cell) noexcept
        {
            return m_isGround.at(cell.y * m_dimsInCells.x + cell.x);
        }

        [[nodiscard]] bool At(glm::i32vec2 const cell) const noexcept
        {
            return m_isGround.at(cell.y * m_dimsInCells.x + cell.x);
        }
    };
}

#endif// GAME_GRID

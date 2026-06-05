#ifndef GAME_GRID
#define GAME_GRID

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
    class Grid final
    {
    public:
        Grid(glm::ivec2 logicalWindowDims);
        ~Grid() noexcept;
        Grid(Grid const&) noexcept = delete;
        Grid(Grid &&) noexcept = delete;
        Grid& operator=(Grid const&) noexcept = delete;
        Grid& operator=(Grid &&) noexcept = delete;

        void Render() const;
        // Returns the location(in cells) of the cell the input point(in pixels) is in
        [[nodiscard]] glm::ivec2 GetCellFromPoint(glm::vec2 point) const noexcept;

        // Returns the location(in pixels) of the top left corner of the input cell(in cells)
        [[nodiscard]] glm::vec2 GetCellTopLeft(glm::ivec2 cell) const noexcept;

        // Returns whether the point is in the ground and preserves the current location
        bool TryDigging(glm::ivec2 pointInPx) noexcept;

    private:
        glm::ivec2 m_dimsInPx{};
        glm::ivec2 m_dimsInCells{};
        Engine::Renderer& m_renderer;
        std::function<void()> m_renderFunction{ [this]{ Render(); } };

        // Array representing 2D grid of bools communicating whether each cell is ground or not
        std::vector<bool> m_isGround{};
        uint32_t m_currentCellIdx{};

    };
}

#endif// GAME_GRID

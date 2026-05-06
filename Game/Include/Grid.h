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
        Grid(int32_t cellSideLength, glm::ivec2 logicalWindowDims);
        void Render() const;
        // Returns the location(in cells) of the cell the input point(in pixels) is in
        [[nodiscard]] glm::ivec2 GetCellFromPoint(glm::vec2 point) const noexcept;

        // Returns the location(in pixels) of the top left corner of the input cell(in cells)
        [[nodiscard]] glm::vec2 GetCellTopLeft(glm::ivec2 cell) const noexcept;

        void TryDigging(glm::ivec2 point, int32_t radius) noexcept;

    private:
        int32_t m_cellSideLength;
        glm::ivec2 m_dimsInPx;
        glm::ivec2 m_dimsInCells{};
        Engine::Renderer& m_renderer;
        std::function<void()> m_renderFunction{ [this]{ Render(); } };

    };
}

#endif// GAME_GRID

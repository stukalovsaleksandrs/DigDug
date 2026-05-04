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

    private:
        int32_t m_cellSideLength;
        glm::ivec2 m_dimsInPx;
        glm::ivec2 m_dimsInCells;
        Engine::Renderer& m_renderer;
        std::function<void()> m_renderFunction{ [this]{ Render(); } };

    };
}

#endif// GAME_GRID

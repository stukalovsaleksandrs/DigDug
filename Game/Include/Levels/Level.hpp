#ifndef GAME_LEVEL
#define GAME_LEVEL

// Game
#include "Grid.hpp"
// Engine
#include "Engine/Rendering/Sprite.hpp"
// Third-party
#include <SDL3/SDL_render.h>

namespace Game
{
    class Level final
    {
    public:
        explicit Level(std::string_view path) noexcept;

        void SetActive(bool active) const noexcept;

    void DigCircle(glm::vec2 centerPx) const noexcept;

    // Given a px, gets a cell of this px and returns top left px of this cell
    glm::vec2 GetCellTopLeft(glm::u32vec2 centerPx) const noexcept;

    private:
        SDL_Renderer* m_pSDLRenderer{};

        glm::u32vec2 m_characterSpawnCell{};
        std::vector<glm::u32vec2> m_pookaSpawnCells{},
            m_flygarSpawnCells{}, m_rockSpawnCells{};
        Grid m_grid;
        Engine::Sprite m_maskTexture;

        std::function<void()> m_renderTunnelsFunction{ [this]
        {
            this->m_maskTexture.Render();
        } };

        void ParseFile(std::string_view path);

        void ParseCharacter(std::string_view path, std::string_view line, glm::u32vec2 cell);

        void DigSquare(glm::vec2 topLeftPx, EU::Square::Corners corners = {}) const noexcept;

        void MaskInitialTunnels() const noexcept;
    };
}

#endif// GAME_LEVEL

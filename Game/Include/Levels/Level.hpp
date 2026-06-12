#ifndef GAME_LEVEL
#define GAME_LEVEL

// Game
#include "Grid.hpp"
// Engine
#include "Engine/Components/MovementComponent.hpp"
#include "Engine/Rendering/Sprite.hpp"
#include "Engine/Scene/Scene.hpp"

namespace Game
{
    class PumpComponent;

    class Level final
    {
    public:
        struct Resources final
        {
            std::unique_ptr<Engine::Font> pFont;
            std::unique_ptr<Engine::Sprite> pTaizoHoriSprite, pPookaSprite,
                pGroundSprite, pSkySprite,
                pPumpSprite;
        };

        explicit Level(std::string_view path, Resources const& sharedResources) noexcept;
        ~Level() noexcept;
        Level& operator=(Level const&) noexcept = delete;
        Level& operator=(Level&&) noexcept = delete;
        Level(Level const&) noexcept = delete;
        Level(Level&&) noexcept = delete;

        bool TryDigging(glm::vec2 cellCenterPx) noexcept;

        [[nodiscard]] Grid const& GetGrid() const noexcept { return m_grid; }
        [[nodiscard]] Grid& GetGrid() noexcept { return m_grid; }
        [[nodiscard]] PumpComponent& GetPumpComponent() noexcept;

        [[nodiscard]] glm::u32vec2 GetPlayerCell() const noexcept;

        [[nodiscard]] std::vector<Engine::RenderComponent*> GetEnemyRenderComponents() noexcept;

        void Update() noexcept;

    private:
        Resources const& m_sharedResources;

        glm::u32vec2 m_playerSpawnCell{};
        std::vector<glm::u32vec2> m_pookaSpawnCells{},
            m_flygarSpawnCells{}, m_rockSpawnCells{};

        Grid m_grid;

        Engine::Sprite m_maskTexture;

        Engine::GameObject* m_pPlayer{};

        Engine::GameObject* m_pPump{};
        PumpComponent* m_pPumpComponent{};

        std::function<void()> m_renderTunnelsFunction{ [this]
        {
            this->m_maskTexture.Render();
        } };

        std::unordered_map<char, std::function<void(glm::i32vec2 cell)>> m_charToParsingFunc{};

        Engine::Scene m_scene;

        void ParseFile(std::string_view path) const;
        void ParsePlayer(std::string_view line, glm::u32vec2 cell) const;

        void DigCircle(glm::vec2 centerPx) const noexcept;
        void DigSquare(glm::vec2 topLeftPx, EU::Square::Corners corners = {}) const noexcept;

        void MaskInitialTunnels() const noexcept;

        void SpawnPlayer() noexcept;
        void SpawnPookas() noexcept;
        void SpawnPooka(glm::vec2 topLeft) noexcept;
        void SpawnPump() noexcept;

        Engine::MovementComponent::CanMovePred GetCanMovePred() const noexcept;
    };
}

#endif// GAME_LEVEL

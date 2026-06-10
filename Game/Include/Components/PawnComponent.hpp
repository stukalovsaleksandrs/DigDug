#ifndef GAME_PAWN_COMPONENT
#define GAME_PAWN_COMPONENT

// Engine
#include "Engine/Components/ComponentBase.hpp"
#include "Engine/Core/Observer.hpp"

namespace Engine
{
    class MovementComponent;
    class RenderComponent;
}

namespace Game
{
    class Level;

    class PawnComponent : public Engine::Component, public Engine::Observer
    {
    public:
        struct Dependencies final
        {
            Level& level;
        };

        explicit PawnComponent(Engine::GameObject& owner, Dependencies const&) noexcept;
        void OnNotify(Engine::Event event, const Engine::Subject& caller) noexcept override;
        void Update() noexcept override;

    protected:
        Dependencies m_dependencies;
        Engine::MovementComponent& m_movementComponent;
        Engine::RenderComponent& m_renderComponent;

    private:
        class LocationLerpData final
        {
        public:
            void Reset(glm::vec2 srcLocation, glm::vec2 dstLocation, float totalSec) noexcept;
            // Updates the time and
            [[nodiscard]] std::optional<glm::vec2> Update() noexcept;
            [[nodiscard]] bool IsActive() const noexcept{ return m_active; };

        private:
            float m_dstSec{}, m_currentSec{};
            glm::vec2 m_srcLocation{}, m_dstLocation{};
            bool m_active{};
        } m_locationLerpData;

        // Flips and/or rotates the sprite depending on direction
        void ProcessSpriteOrientation(glm::vec2 direction) const noexcept;

        // Lerps player location to the middle of the cell the player is currently in
        void ConstrainMovementToGrid() noexcept;

        // Returns coordinate of the top left corner of the current cell
        [[nodiscard]] glm::vec2 GetCurrentCellTopLeft() const noexcept;
    };

}

#endif// GAME_PAWN_COMPONENT

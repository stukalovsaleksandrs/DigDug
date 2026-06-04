#ifndef PLAYER_CONTROLLER
#define PLAYER_CONTROLLER

// Game
#include "Constants.hpp"
#include "PlayerStateMachine.hpp"
#include "Grid.hpp"
// Engine
#include "Engine/Components/ComponentBase.hpp"
#include "Engine/Core/Observer.hpp"
#include "Engine/InputManager.hpp"

namespace Engine
{
    class AnimationComponent;
}

namespace Game
{
    class MovementComponent;
    class PlayerComponent : public Engine::Component, public Engine::Observer
    {
    public:
        struct Dependencies final
        {
            Grid& grid;
        };

        Engine::Subject subject;

        explicit PlayerComponent(Engine::GameObject& owner, Dependencies const&) noexcept;
        ~PlayerComponent() noexcept override;
        PlayerComponent(PlayerComponent&&) noexcept = delete;
        PlayerComponent(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent const&) noexcept = delete;
        PlayerComponent& operator=(PlayerComponent&&) noexcept = delete;

        void Update() noexcept override;

        void BindInput() noexcept;
        void UnbindInput() const noexcept;

        void OnNotify(Engine::Event event, Engine::Subject const& caller) noexcept override;

        [[nodiscard]] uint32_t GetPoints() const noexcept{ return m_points; };
        void AddPoints(uint32_t points) noexcept;;

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
        Dependencies const m_dependencies;

        uint32_t m_points{};
        Engine::MovementComponent& m_movementComponent;
        Engine::RenderComponent& m_renderComponent;

        Engine::Action m_upAction{SDL_SCANCODE_W, Engine::InputType::held};
        Engine::Action m_leftAction{SDL_SCANCODE_A, Engine::InputType::held};
        Engine::Action m_downAction{SDL_SCANCODE_S, Engine::InputType::held};
        Engine::Action m_rightAction{SDL_SCANCODE_D, Engine::InputType::held};
        Engine::Action m_pointAction{SDL_SCANCODE_P, Engine::InputType::released};

        Engine::Event m_onPointsIncreased{ std::to_underlying(EventType::OnPointsIncreased) };
        Engine::Event m_onCollected5Points{ std::to_underlying(EventType::OnCollected5Points) };

        Player::StateMachine m_stateMachine;

        // Flips and/or rotates the sprite depending on direction
        void ProcessSpriteOrientation(glm::vec2 direction) const noexcept;

        // Lerps player location to the middle of the cell the player is currently in
        void ConstrainMovementToGrid() noexcept;

        // Returns coordinate of the top left corner of the current cell
        [[nodiscard]] glm::vec2 GetCurrentCellTopLeft() const noexcept;
    };
}

#endif

#ifndef GAME_PLAYER_CONTROLLER
#define GAME_PLAYER_CONTROLLER

// Game
#include "Utils.hpp"
#include "FSM/PlayerStates.hpp"
#include "Grid.hpp"
// Engine
#include "PawnComponent.hpp"
#include "Engine/Core/Observer.hpp"

namespace Engine
{
    class AnimationComponent;
}

namespace Game
{
    class PlayerComponent final : public PawnComponent, public Engine::Subject
    {
    public:
        explicit PlayerComponent(Engine::GameObject& owner, Dependencies const&) noexcept;

        void Update() noexcept override;

        [[nodiscard]] uint32_t GetPoints() const noexcept{ return m_points; };
        void AddPoints(uint32_t points) noexcept;

    private:
        uint32_t m_points{};

        Engine::Event m_onPointsIncreased{ std::to_underlying(EventType::OnPointsIncreased) };
        Engine::Event m_onCollected5Points{ std::to_underlying(EventType::OnCollected5Points) };

        FSM m_playerStateMachine;
    };
}

#endif// GAME_PLAYER_CONTROLLER

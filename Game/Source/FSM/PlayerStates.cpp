// Game
#include "Utils.hpp"
#include "FSM/PlayerStates.hpp"
#include "Levels/LevelManager.hpp"
// Engine
#include "Engine/Components/AnimationComponent.hpp"
#include "Engine/Components/MovementComponent.hpp"
#include "Engine/Scene/GameObject.hpp"

namespace Game
{
#pragma region PlayerStateBase
    bool Player::State::PlayerStateBase::TryDigging() const noexcept
    {
        auto const worldPosition{m_dependencies.owner.GetWorldLocation()};
        return m_dependencies.level.TryDigging(
            worldPosition + topLeftToCenterOffset
        );
    }
#pragma endregion PlayerStateBase

#pragma region Idle
    Player::State::Idle::Idle(Dependencies const& dependencies) noexcept
        : PlayerStateBase{dependencies}{}

    void Player::State::Idle::OnEnter() noexcept
    {
        // Just using the first frame statically
        m_dependencies.animationComponent.ChangeAnimation(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(tileSideLength),
                static_cast<float>(tileSideLength)},
            1
        );
    }

    StateType Player::State::Idle::Update() noexcept
    {
        if (IsMoving()) return typeid(Walking);
        return std::nullopt;
    }
#pragma endregion Idle

#pragma region Walking
    Player::State::Walking::Walking(Dependencies const& dependencies) noexcept
        : PlayerStateBase{dependencies} {}

    void Player::State::Walking::OnEnter() noexcept
    {
        m_dependencies.animationComponent.ChangeAnimation(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(tileSideLength),
                static_cast<float>(tileSideLength)},
            2
        );
    }

    StateType Player::State::Walking::Update() noexcept
    {
        // Not moving -> idle
        if (auto& movementComponent{ m_dependencies.movementComponent };
            !movementComponent.IsMoving())
            return typeid(Idle);

        // Digging -> switching state
        if (TryDigging()) return typeid(Digging);

        return std::nullopt;
    }
#pragma endregion Walking

#pragma region Digging
    Player::State::Digging::Digging(Dependencies const& dependencies) noexcept
        : PlayerStateBase{dependencies}
    {}

    void Player::State::Digging::OnEnter() noexcept
    {
        // TODO: Change sprite sheet view
    }

    StateType Player::State::Digging::Update() noexcept
    {
        if (!(TryDigging() and IsMoving())) return typeid(Idle);
        return std::nullopt;
    }

#pragma endregion Digging

}

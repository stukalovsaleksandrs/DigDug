// Game
#include "Utils.hpp"
#include "Commands.hpp"
#include "FSM/PlayerStates.hpp"
#include "Levels/LevelManager.hpp"
// Engine
#include "Engine/Components/AnimationComponent.hpp"
#include "Engine/Components/MovementComponent.hpp"
#include "Engine/Scene/GameObject.hpp"

namespace Game
{
#pragma region PlayerStateBase
    Player::State::PlayerStateBase::PlayerStateBase(Dependencies const& dependencies)
        : m_dependencies{ dependencies }
        , m_movementComponent{ *m_dependencies.owner.GetComponent<Engine::MovementComponent>() }
        , m_animationComponent{ *m_dependencies.owner.GetComponent<Engine::AnimationComponent>() }
    {}

    bool Player::State::PlayerStateBase::TryDigging() const noexcept
    {
        auto const worldPosition{m_dependencies.owner.GetWorldLocation()};
        return m_dependencies.level.TryDigging(
            worldPosition + topLeftToCenterOffset
        );
    }

    void Player::State::PlayerStateBase::BindAllInput(PlayerComponent& playerComponent) const noexcept
    {
        auto makeMoveCommand{
            [this](glm::vec2 direction)
            {
                return std::make_unique<Engine::MoveCommand>(
                    m_movementComponent,
                    direction
                );
            }
        };

        Engine::InputManager& inputManager{ Engine::InputManager::GetInstance() };
        //// Keyboard
        inputManager.Bind(m_keyboardUp, makeMoveCommand(glm::vec2{ 0.f, -1.f }));
        inputManager.Bind(m_keyboardLeft, makeMoveCommand(glm::vec2{ -1.f, 0.f }));
        inputManager.Bind(m_keyboardDown, makeMoveCommand(glm::vec2{ 0.f, 1.f }));
        inputManager.Bind(m_keyboardRight, makeMoveCommand(glm::vec2{ 1.f, 0.f }));
        inputManager.Bind(m_keyboardAttackAction, std::make_unique<AttackCommand>(playerComponent));

        // Gamepad
        inputManager.Bind(m_gamepadUp, makeMoveCommand(glm::vec2{ 0.f, -1.f }));
        inputManager.Bind(m_gamepadLeft, makeMoveCommand(glm::vec2{ -1.f, 0.f }));
        inputManager.Bind(m_gamepadDown, makeMoveCommand(glm::vec2{ 0.f, 1.f }));
        inputManager.Bind(m_gamepadRight, makeMoveCommand(glm::vec2{ 1.f, 0.f }));
        inputManager.Bind(m_gamepadAttackAction, std::make_unique<AttackCommand>(playerComponent));
    }

    void Player::State::PlayerStateBase::UnbindAllInput() const noexcept
    {
        Engine::InputManager& inputManager{ Engine::InputManager::GetInstance() };

        // Keyboard
        inputManager.Unbind(m_keyboardUp);
        inputManager.Unbind(m_keyboardLeft);
        inputManager.Unbind(m_keyboardDown);
        inputManager.Unbind(m_keyboardRight);
        inputManager.Unbind(m_keyboardPointAction);

        // Gamepad
        inputManager.Unbind(m_gamepadUp);
        inputManager.Unbind(m_gamepadLeft);
        inputManager.Unbind(m_gamepadDown);
        inputManager.Unbind(m_gamepadRight);
    }
#pragma endregion PlayerStateBase

#pragma region Idle
    Player::State::Idle::Idle(Dependencies const& dependencies, PlayerComponent& playerComponent) noexcept
        : PlayerStateBase{dependencies}
    {
        BindAllInput(playerComponent);
    }

    void Player::State::Idle::OnEnter() noexcept
    {
        // Just using the first frame statically
        m_animationComponent.ChangeAnimation(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(tileSideLength),
                static_cast<float>(tileSideLength)},
            1
        );
    }

    StateType Player::State::Idle::Update() noexcept
    {
        if (m_movementComponent.IsMoving()) return typeid(Walking);
        return std::nullopt;
    }
#pragma endregion Idle

#pragma region Walking
    Player::State::Walking::Walking(Dependencies const& dependencies) noexcept
        : PlayerStateBase{dependencies} {}

    void Player::State::Walking::OnEnter() noexcept
    {
        m_animationComponent.ChangeAnimation(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(tileSideLength),
                static_cast<float>(tileSideLength)},
            2
        );
    }

    StateType Player::State::Walking::Update() noexcept
    {
        // Not moving -> idle
        if (auto const& movementComponent{ m_movementComponent };
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
        if (!(TryDigging() and m_movementComponent.IsMoving())) return typeid(Idle);
        return std::nullopt;
    }

#pragma endregion Digging

#pragma region Attacking

    StateType Player::State::Attacking::Update() noexcept
    {
        return std::nullopt;
    }

    void Player::State::Attacking::OnEnter() noexcept
    {
        BindAttackInput();
    }

    void Player::State::Attacking::OnExit() noexcept
    {
    }

    void Player::State::Attacking::BindAttackInput() const noexcept
    {
        auto& inputManager{ Engine::InputManager::GetInstance() };
        inputManager.Bind(m_keyboardAttackAction, std::make_unique<AttackCommand>(m_dependencies.playerComponent));
        inputManager.Bind(m_gamepadAttackAction, std::make_unique<AttackCommand>(m_dependencies.playerComponent));
    }

    void Player::State::Attacking::UnbindAttackInput() const noexcept
    {
        auto& inputManager{ Engine::InputManager::GetInstance() };
        inputManager.Unbind(m_keyboardAttackAction);
        inputManager.Unbind(m_gamepadAttackAction);
    }

#pragma endregion Attacking

#pragma region Dying
    Player::State::Dying::Dying(Dependencies const& dependencies) noexcept
        : PlayerStateBase{dependencies}
    {}

    void Player::State::Dying::OnExit() noexcept
    {
        UnbindAllInput();
    }
#pragma endregion Dying

}

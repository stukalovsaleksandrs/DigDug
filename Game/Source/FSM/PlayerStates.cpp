// Game
#include "Utils.hpp"
#include "Commands.hpp"
#include "FSM/PlayerStates.hpp"
#include "Levels/LevelManager.hpp"
#include "Components/PumpComponent.hpp"
// Engine
#include "Components/AIComponent.hpp"
#include "Engine/Components/AnimationComponent.hpp"
#include "Engine/Components/MovementComponent.hpp"
#include "Engine/Scene/GameObject.hpp"
#include "Engine/Utils/Timer.hpp"
// Standard
#include <print>

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

    void Player::State::PlayerStateBase::BindAllInput(FSM& fsm) const noexcept
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
        // Keyboard
        inputManager.Bind(m_keyboardUp, makeMoveCommand(glm::vec2{ 0.f, -1.f }));
        inputManager.Bind(m_keyboardLeft, makeMoveCommand(glm::vec2{ -1.f, 0.f }));
        inputManager.Bind(m_keyboardDown, makeMoveCommand(glm::vec2{ 0.f, 1.f }));
        inputManager.Bind(m_keyboardRight, makeMoveCommand(glm::vec2{ 1.f, 0.f }));
        inputManager.Bind(m_keyboardAttackAction, std::make_unique<AttackCommand>(fsm));
        // Gamepad
        inputManager.Bind(m_gamepadUp, makeMoveCommand(glm::vec2{ 0.f, -1.f }));
        inputManager.Bind(m_gamepadLeft, makeMoveCommand(glm::vec2{ -1.f, 0.f }));
        inputManager.Bind(m_gamepadDown, makeMoveCommand(glm::vec2{ 0.f, 1.f }));
        inputManager.Bind(m_gamepadRight, makeMoveCommand(glm::vec2{ 1.f, 0.f }));
        inputManager.Bind(m_gamepadAttackAction, std::make_unique<AttackCommand>(fsm));
    }

    void Player::State::PlayerStateBase::UnbindAllInput() const noexcept
    {
        Engine::InputManager& inputManager{ Engine::InputManager::GetInstance() };

        // Keyboard
        inputManager.Unbind(m_keyboardUp);
        inputManager.Unbind(m_keyboardLeft);
        inputManager.Unbind(m_keyboardDown);
        inputManager.Unbind(m_keyboardRight);

        // Gamepad
        inputManager.Unbind(m_gamepadUp);
        inputManager.Unbind(m_gamepadLeft);
        inputManager.Unbind(m_gamepadDown);
        inputManager.Unbind(m_gamepadRight);
    }

    void Player::State::PlayerStateBase::BindAttackInput() const noexcept
    {
        auto& inputManager{ Engine::InputManager::GetInstance() };
        inputManager.Bind(m_keyboardAttackAction, std::make_unique<AttackCommand>(m_dependencies.fsm));
        inputManager.Bind(m_gamepadAttackAction, std::make_unique<AttackCommand>(m_dependencies.fsm));
    }

    void Player::State::PlayerStateBase::UnbindAttackInput() const noexcept
    {
        auto& inputManager{ Engine::InputManager::GetInstance() };
        inputManager.Unbind(m_keyboardAttackAction);
        inputManager.Unbind(m_gamepadAttackAction);
    }

    StateType Player::State::PlayerStateBase::ProcessGameAction(GameAction const action) noexcept
    {
        switch (action)
        {
        case GameAction::Attack:
            return typeid(Throw);
        default: ;
        }
        return StateBase::ProcessGameAction(action);
    }

#pragma endregion PlayerStateBase

#pragma region Idle
    Player::State::Idle::Idle(Dependencies const& dependencies) noexcept
        : PlayerStateBase{dependencies}
    {
        BindAllInput(dependencies.fsm);
    }

    void Player::State::Idle::OnEnter() noexcept
    {
        // Just using the first frame statically
        m_animationComponent.ChangeAnimation(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(i32tileSideLength),
                static_cast<float>(i32tileSideLength)},
            1, 0.f
        );
    }

    StateType Player::State::Idle::Update() noexcept
    {
        if (m_movementComponent.IsMoving()) return typeid(Walk);
        return std::nullopt;
    }
#pragma endregion Idle

#pragma region Walking
    Player::State::Walk::Walk(Dependencies const& dependencies) noexcept
        : PlayerStateBase{dependencies} {}

    void Player::State::Walk::OnEnter() noexcept
    {
        m_animationComponent.ChangeAnimation(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(i32tileSideLength),
                static_cast<float>(i32tileSideLength)},
            2, 0.1f
        );
    }

    StateType Player::State::Walk::Update() noexcept
    {
        // Not moving -> idle
        if (auto const& movementComponent{ m_movementComponent };
            !movementComponent.IsMoving())
            return typeid(Idle);

        // Digging -> switching state
        if (TryDigging()) return typeid(Dig);

        return std::nullopt;
    }

#pragma endregion Walking

#pragma region Digging
    Player::State::Dig::Dig(Dependencies const& dependencies) noexcept
        : PlayerStateBase{dependencies}
    {}

    void Player::State::Dig::OnEnter() noexcept
    {
        // TODO: Change sprite sheet view
    }

    StateType Player::State::Dig::Update() noexcept
    {
        if (!(TryDigging() and m_movementComponent.IsMoving())) return typeid(Idle);
        return std::nullopt;
    }

#pragma endregion Digging

#pragma region Throw
    Player::State::Throw::Throw(Dependencies const& dependencies, PumpComponent& pumpComponent)
        : PlayerStateBase{ dependencies }
        , m_pumpComponent{ pumpComponent }
    {
        m_pumpComponent.SetDuration(m_durationSec);
        m_pumpComponent.SetActive(false);
    }

    StateType Player::State::Throw::Update() noexcept
    {
        m_currentSec += Engine::Timer::GetInstance().GetDeltaSec();

        if (auto const result{ ProcessCollisions()}; result != std::nullopt)
        {
            std::println("Current time: {}/{}", m_currentSec, m_durationSec);

            m_pumpComponent.Pause();

             return result;
        }

        if (m_currentSec > m_durationSec)
        {
            BindAttackInput();
            m_movementComponent.SetActive(true);
            m_pumpComponent.SetActive(false);
            return typeid(Idle);
        }
        return std::nullopt;
    }

    void Player::State::Throw::OnEnter() noexcept
    {
        m_pumpComponent.SetActive(true);
        UnbindAttackInput();
        m_movementComponent.SetActive(false);
        m_currentSec = 0.f;
    }

    void Player::State::Throw::OnExit() noexcept
    {}

    StateType Player::State::Throw::ProcessCollisions() const noexcept
    {
        SDL_FRect const pumpDstRect{ m_pumpComponent.GetDstRect() };
        for (Engine::RenderComponent const * const pEnemyRenderComponent : m_dependencies.level.GetEnemyRenderComponents())
        {
            assert(pEnemyRenderComponent);
            glm::vec2 const enemyTopLeft{ pEnemyRenderComponent->GetOwner().GetWorldLocation() };
            SDL_FRect const enemyDstRect{
                enemyTopLeft.x, enemyTopLeft.y,
                pEnemyRenderComponent->dstDims.x, pEnemyRenderComponent->dstDims.y
            };

            if (Engine::Utils::Intersect(pumpDstRect, enemyDstRect))
            {
                // 1. Getting enemy game object, getting AIComponent
                AIComponent& enemyAIComponent{
                    *pEnemyRenderComponent->GetOwner().GetComponent<AIComponent>()
                };
                // 2. Setting the state for the enemy to get pumped up
                enemyAIComponent.OnCaught();
                // 3. Setting pumping state for the player character
                return typeid(Pump);
            }
        }
        return std::nullopt;
    }
#pragma endregion Throw

#pragma region Pump
    Player::State::Pump::Pump(Dependencies const& dependencies, PumpComponent& pumpComponent)
        : PlayerStateBase{ dependencies }
        , m_pumpComponent{ pumpComponent }
    {}

    StateType Player::State::Pump::Update() noexcept
    {
        return PlayerStateBase::Update();
    }

    void Player::State::Pump::OnEnter() noexcept
    {
        PlayerStateBase::OnEnter();
        // Playing pumping animation
        m_animationComponent.ChangeAnimation(
            SDL_FRect{2 * ftileSideLength, 0.f,
                ftileSideLength,
                ftileSideLength},
            2, 0.3f
        );
    }

    void Player::State::Pump::OnExit() noexcept
    {
        PlayerStateBase::OnExit();
        BindAttackInput();
        m_movementComponent.SetActive(true);
        m_pumpComponent.SetActive(false);
    }
#pragma endregion Pump

#pragma region Dying
    Player::State::Die::Die(Dependencies const& dependencies) noexcept
        : PlayerStateBase{dependencies}
    {}

    void Player::State::Die::OnExit() noexcept
    {
        UnbindAllInput();
    }
#pragma endregion Dying

}

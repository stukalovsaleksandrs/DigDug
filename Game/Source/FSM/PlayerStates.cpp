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

    StateType Player::State::PlayerStateBase::Update() noexcept
    {
        return StateBase::Update();
    }

    bool Player::State::PlayerStateBase::TryDigging() const noexcept
    {
        auto const worldPosition{m_dependencies.owner.GetWorldTopLeft()};
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
        inputManager.Bind(m_keyboardAttackStartAction, std::make_unique<AttackStartCommand>(fsm));
        inputManager.Bind(m_keyboardAttackStopAction, std::make_unique<AttackStopCommand>());
        // Gamepad
        inputManager.Bind(m_gamepadUp, makeMoveCommand(glm::vec2{ 0.f, -1.f }));
        inputManager.Bind(m_gamepadLeft, makeMoveCommand(glm::vec2{ -1.f, 0.f }));
        inputManager.Bind(m_gamepadDown, makeMoveCommand(glm::vec2{ 0.f, 1.f }));
        inputManager.Bind(m_gamepadRight, makeMoveCommand(glm::vec2{ 1.f, 0.f }));
        inputManager.Bind(m_gamepadAttackStartAction, std::make_unique<AttackStartCommand>(fsm));
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
        inputManager.Bind(m_keyboardAttackStartAction, std::make_unique<AttackStartCommand>(m_dependencies.fsm));
        inputManager.Bind(m_gamepadAttackStartAction, std::make_unique<AttackStartCommand>(m_dependencies.fsm));
    }

    void Player::State::PlayerStateBase::UnbindAttackInput() const noexcept
    {
        auto& inputManager{ Engine::InputManager::GetInstance() };
        inputManager.Unbind(m_keyboardAttackStartAction);
        inputManager.Unbind(m_gamepadAttackStartAction);
    }

    bool Player::State::PlayerStateBase::CollidesWithEnemy() const noexcept
    {
        glm::vec2 const playerTopLeft{ m_dependencies.owner.GetWorldTopLeft() };
        float constexpr padding{ 8 };
        SDL_FRect const playerDstRect{
            playerTopLeft.x + padding, playerTopLeft.y + padding,
            ftileSideLengthPx - padding, ftileSideLengthPx - padding
        };

        for (Engine::RenderComponent const * const pEnemyRenderComponent : m_dependencies.level.GetEnemyRenderComponents())
        {
            assert(pEnemyRenderComponent);
            glm::vec2 const enemyTopLeft{ pEnemyRenderComponent->GetOwner().GetWorldTopLeft() };
            SDL_FRect const enemyDstRect{
                enemyTopLeft.x, enemyTopLeft.y,
                pEnemyRenderComponent->dstDims.x, pEnemyRenderComponent->dstDims.y
            };

            if (Engine::Utils::Intersect(playerDstRect, enemyDstRect)) return true;
        }

        return false;
    }

    StateType Player::State::PlayerStateBase::ProcessGameEvent(EventType const type) noexcept
    {
        switch (type)
        {
        case EventType::OnThrow:
            return typeid(Throw);
        default: ;
        }
        return StateBase::ProcessGameEvent(type);
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
        m_dependencies.owner.SetActive(true);
        m_movementComponent.SetActive(true);
        BindAllInput(m_dependencies.fsm);
        // Just using the first frame statically
        m_animationComponent.ChangeSource(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(i32tileSideLengthPx),
                static_cast<float>(i32tileSideLengthPx)},
            1, 0.f
        );
    }

    StateType Player::State::Idle::Update() noexcept
    {
        if (CollidesWithEnemy()) return typeid(Die);

        if (m_movementComponent.IsMoving()) return typeid(Walk);
        return std::nullopt;
    }
#pragma endregion Idle

#pragma region Walking
    Player::State::Walk::Walk(Dependencies const& dependencies) noexcept
        : PlayerStateBase{dependencies} {}

    void Player::State::Walk::OnEnter() noexcept
    {
        m_animationComponent.ChangeSource(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(i32tileSideLengthPx),
                static_cast<float>(i32tileSideLengthPx)},
            2, 0.1f
        );
    }

    StateType Player::State::Walk::Update() noexcept
    {
        if (CollidesWithEnemy()) return typeid(Die);

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
        if (CollidesWithEnemy()) return typeid(Die);

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
        if (CollidesWithEnemy()) return typeid(Die);

        m_currentSec += Engine::Timer::GetInstance().GetDeltaSec();

        if (auto const result{ ProcessCollisions()}; result != std::nullopt)
        {
            // std::println("Current time: {}/{}", m_currentSec, m_durationSec);

            m_pumpComponent.SetPaused(true);

             return result;
        }

        if (m_currentSec > m_durationSec)
        {
            // BindAttackInput();
            m_movementComponent.SetActive(true);
            m_pumpComponent.SetActive(false);
            return typeid(Idle);
        }
        return std::nullopt;
    }

    void Player::State::Throw::OnEnter() noexcept
    {
        m_pumpComponent.SetActive(true);
        m_pumpComponent.SetPaused(false);
        // UnbindAttackInput();
        m_movementComponent.SetActive(false);
        m_currentSec = 0.f;
    }

    void Player::State::Throw::OnExit() noexcept
    {
        m_pumpComponent.SetPaused(false);
        m_pumpComponent.SetActive(false);
        m_movementComponent.SetActive(true);
    }

    StateType Player::State::Throw::ProcessCollisions() const noexcept
    {
        if (IsCollidingWithGround())
        {
            // BindAttackInput();
            return typeid(Walk);
        }
        return ProcessPumpEnemyCollisions();
    }

    bool Player::State::Throw::IsCollidingWithGround() const noexcept
    {
        // Don't test until the pump has actually extended out of the player tile.
        // i32tileSideLengthPx is the minimum meaningful extension.
        if (m_pumpComponent.GetCurrentWidthPx() < ftileSideLengthPx)
            return false;

        Grid const& grid { m_dependencies.level.GetGrid() };
        glm::vec2 const leadingPt { m_pumpComponent.GetLeadingEdgePoint() };
        return grid.IsGround(grid.GetCellFromPoint(leadingPt));
    }

    StateType Player::State::Throw::ProcessPumpEnemyCollisions() const noexcept
    {
        SDL_FRect const pumpDstRect{ m_pumpComponent.GetDstRect() };
        for (Engine::RenderComponent const * const pEnemyRenderComponent : m_dependencies.level.GetEnemyRenderComponents())
        {
            assert(pEnemyRenderComponent);
            glm::vec2 const enemyTopLeft{ pEnemyRenderComponent->GetOwner().GetWorldTopLeft() };
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
        if (CollidesWithEnemy()) return typeid(Die);
        return PlayerStateBase::Update();
    }

    void Player::State::Pump::OnEnter() noexcept
    {
        PlayerStateBase::OnEnter();
        m_pumpComponent.SetActive(true);
        m_pumpComponent.SetPaused(true);
        m_movementComponent.SetActive(false);
        // Playing pumping animation
        m_animationComponent.ChangeSource(
            SDL_FRect{2 * ftileSideLengthPx, 0.f,
                ftileSideLengthPx,
                ftileSideLengthPx},
            2, 0.3f
        );
    }

    void Player::State::Pump::OnExit() noexcept
    {
        PlayerStateBase::OnExit();
        // BindAttackInput();
        m_movementComponent.SetActive(true);
        m_pumpComponent.SetPaused(false);
        m_pumpComponent.SetActive(false);
    }

    StateType Player::State::Pump::ProcessGameEvent(EventType const type) noexcept
    {
        switch (type)
        {
        case EventType::OnEnemyDied:
            return typeid(Idle);
        default:
            break;
        }
        return PlayerStateBase::ProcessGameEvent(type);
    }
#pragma endregion Pump

#pragma region Die
    Player::State::Die::Die(
        Dependencies const& dependencies
    ) noexcept
        : PlayerStateBase{dependencies}
    {}

    void Player::State::Die::OnEnter() noexcept
    {
        m_currentSec = 0.f;
        PlayerStateBase::OnEnter();
        UnbindAllInput();
        m_animationComponent.ChangeSource(
            SDL_FRect{
            0.f,
            ftileSideLengthPx,
            ftileSideLengthPx,
            ftileSideLengthPx
            }, m_frameCount, m_secPerFrame
        );
        m_dependencies.level.GetLives().TakeDamage();
    }

    StateType Player::State::Die::Update() noexcept
    {
        m_currentSec += Engine::Timer::GetInstance().GetDeltaSec();
        if (m_currentSec >= m_secPerFrame * m_frameCount)
        {
            m_dependencies.level.OnPlayerCharacterDied();
            return typeid(Idle);
        }
        return std::nullopt;
    }

    void Player::State::Die::OnExit() noexcept
    {
        BindAllInput(m_dependencies.fsm);
        m_dependencies.owner.GetComponent<Engine::RenderComponent>()->SetRotation(0.f);
    }
#pragma endregion Die

}

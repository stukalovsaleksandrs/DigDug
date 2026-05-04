// Game
#include "Constants.h"
#include "PlayerStateMachine.h"

#include <print>

namespace Game
{
    /************************
     * Idle
     ************************/
    Player::State::Idle::Idle(Dependencies const& dependencies) noexcept
        : m_dependencies{ dependencies }
    {}

    void Player::State::Idle::OnEnter() noexcept
    {
        m_dependencies.animationComponent.ChangeAnimation(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(characterDims.x),
                static_cast<float>(characterDims.y)},
            1
        );
}

    Engine::pState Player::State::Idle::Update() noexcept
    {
       if (m_dependencies.movementComponent.IsMoving())
       {
           std::println("Character is moving, switching to walking");
       }

        return m_dependencies.movementComponent.IsMoving()
        ?  std::make_unique<Walking>(m_dependencies)
        : nullptr;
    }

    /************************
     * Walking
     ************************/
    Player::State::Walking::Walking(Dependencies const& dependencies) noexcept
        : m_dependencies{ dependencies }
    {}

    void Player::State::Walking::OnEnter() noexcept
    {
        m_dependencies.animationComponent.ChangeAnimation(
            SDL_FRect{0.f, 0.f,
                static_cast<float>(characterDims.x),
                static_cast<float>(characterDims.y)},
            2
        );
    }

    Engine::pState Player::State::Walking::Update() noexcept
    {
        return m_dependencies.movementComponent.IsMoving()
        ? nullptr
        :  std::make_unique<Idle>(m_dependencies);
    }

    /************************
     * PlayerStateMachine
     ************************/
    Player::StateMachine::StateMachine(State::Dependencies const& dependencies) noexcept
        : m_pCurrentState{ std::make_unique<State::Idle>(dependencies) }
    {
        m_pCurrentState->OnEnter();
    }

    void Player::StateMachine::Update() noexcept
    {
        TryChangingState(
            m_pCurrentState->Update()
        );
    }

    void Player::StateMachine::TryChangingState(Engine::pState pState)
    {
        if (!pState) return;
        if (typeid(*pState) == typeid(*m_pCurrentState)) return;
        m_pCurrentState->OnExit();
        m_pCurrentState = std::move(pState);
        m_pCurrentState->OnEnter();
    }
}

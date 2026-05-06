#ifndef GAME_PLAYER_STATES
#define GAME_PLAYER_STATES

// Engine
#include "Engine/State.h"
#include "Engine/Components/AnimationComponent.h"
#include "Engine/Components/MovementComponent.h"
// Standard
#include <memory>

namespace Game::Player
{
    namespace State
    {
        struct Dependencies final
        {
            Engine::AnimationComponent& animationComponent;
            Engine::MovementComponent const& movementComponent;
        };
        class Idle final : public Engine::IState
        {
        public:
            explicit Idle(Dependencies const&) noexcept;
            void OnEnter() noexcept override;
            void OnExit() noexcept override{}
            Engine::pState Update() noexcept override;

        private:
            Dependencies m_dependencies;

        };

        class Walking final : public Engine::IState
        {
        public:
            explicit Walking(Dependencies const&) noexcept;
            void OnEnter() noexcept override;
            void OnExit() noexcept override{}
            Engine::pState Update() noexcept override;

        private:
            Dependencies m_dependencies;

        };
    }

    class StateMachine final
    {
    public:
        explicit StateMachine(State::Dependencies const&) noexcept;
        void Update() noexcept;

    private:
        Engine::pState m_pCurrentState;

        void TryChangingState(Engine::pState);

    };
}

#endif

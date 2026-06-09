#ifndef GAME_PLAYER_STATES
#define GAME_PLAYER_STATES

// Game
#include "../Grid.hpp"
// Engine
#include "Engine/Components/AnimationComponent.hpp"
#include "Engine/Components/MovementComponent.hpp"
// Standard
#include <memory>
#include <typeindex>

namespace Game
{
    class LevelManager;
}

namespace Game::Player
{
    namespace State
    {
        struct Dependencies final
        {
            Engine::AnimationComponent& animationComponent;
            Engine::MovementComponent const& movementComponent;
            Engine::GameObject& owner;// GetWorldLocation is not const
            LevelManager const& levelManager;
        };

        using StateType = std::optional<std::type_index>;
        class StateBase
        {
        public:
            explicit StateBase(Dependencies const& dependencies) noexcept
                : m_dependencies{ dependencies }{}
            virtual ~StateBase() = default;
            virtual StateType Update() noexcept = 0;
            virtual void OnEnter() noexcept = 0;
            virtual void OnExit() noexcept = 0;

        protected:
            Dependencies m_dependencies;

            [[nodiscard]] bool IsMoving() const noexcept
            {
                return m_dependencies.movementComponent.IsMoving();
            }
        };

        class PlayerStateBase : public StateBase
        {
        public:
            explicit PlayerStateBase(Dependencies const& dependencies)
                : StateBase{ dependencies }{}

        protected:
            // true - digging, false - not digging
            [[nodiscard]] bool TryDigging() const noexcept;
        };

        class Idle final : public PlayerStateBase
        {
        public:
            explicit Idle(Dependencies const&) noexcept;
            void OnEnter() noexcept override;
            void OnExit() noexcept override{}
            StateType Update() noexcept override;
        };

        class Walking final : public PlayerStateBase
        {
        public:
            explicit Walking(Dependencies const&) noexcept;
            void OnEnter() noexcept override;
            void OnExit() noexcept override{}
            StateType Update() noexcept override;
        };

        class Digging final : public PlayerStateBase
        {
        public:
            explicit Digging(Dependencies const&) noexcept;
            ~Digging() noexcept override;
            Digging(Digging const&) noexcept = delete;
            Digging(Digging&&) noexcept = delete;
            Digging& operator=(Digging&&) noexcept = delete;
            Digging& operator=(Digging const&) noexcept = delete;

            void OnEnter() noexcept override;
            StateType Update() noexcept override;
            void OnExit() noexcept override{};

        private:
            void Dig() const noexcept;

        };
    }

    class FSM final
    {
    public:
        using States = std::unordered_map<std::type_index, std::unique_ptr<State::StateBase>>;

        explicit FSM(States&& initialStates) noexcept;
        void Update() noexcept;

    private:
        States m_states;
        // Not a ref, bc, when changing state, assigning ref calls copy assignment
        State::StateBase* m_pCurrentState;
        void TryChangingState(State::StateType);

        [[nodiscard]] State::StateBase* StatesAt(std::type_index const& typeIdx) const noexcept
        {
            return m_states.at(typeIdx).get();
        }
    };
}

#endif

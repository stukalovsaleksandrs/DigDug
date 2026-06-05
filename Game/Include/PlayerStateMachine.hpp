#ifndef GAME_PLAYER_STATES
#define GAME_PLAYER_STATES

// Game
#include "Grid.hpp"
// Engine
#include "Engine/Components/AnimationComponent.hpp"
#include "Engine/Components/MovementComponent.hpp"
// Third-party
#include <SDL3/SDL_render.h>
// Standard
#include <memory>
#include <typeindex>

namespace Game::Player
{
    namespace State
    {
        struct Dependencies final
        {
            Engine::AnimationComponent& animationComponent;
            Engine::MovementComponent const& movementComponent;
            Grid& grid;
            Engine::GameObject& owner;// GetWorldLocation is not const
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

            // true - digging, false - not digging
            [[nodiscard]] bool TryDigging() const noexcept;
        };

        class Idle final : public StateBase
        {
        public:
            explicit Idle(Dependencies const&) noexcept;
            void OnEnter() noexcept override;
            void OnExit() noexcept override{}
            StateType Update() noexcept override;
        };

        class Walking final : public StateBase
        {
        public:
            explicit Walking(Dependencies const&) noexcept;
            void OnEnter() noexcept override;
            void OnExit() noexcept override{}
            StateType Update() noexcept override;
        };

        class Digging final : public StateBase
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
            void RenderTunnels() const noexcept;

        private:
            std::function<void()> m_renderTunnelsFunction{ [this]{this->RenderTunnels();} };

            SDL_Texture* m_maskTexture{};
            SDL_Renderer* m_pSDLRenderer{};

            void Dig() const noexcept;
        };
    }

    class StateMachine final
    {
    public:
        explicit StateMachine(State::Dependencies const&) noexcept;
        void Update() noexcept;

    private:
        std::unordered_map<std::type_index, std::unique_ptr<State::StateBase>> m_states;
        State::StateBase* m_pCurrentState;// Not a ref, bc assigning ref calls copy assignment
        void TryChangingState(State::StateType);

        [[nodiscard]] State::StateBase* StatesAt(std::type_index const& typeIdx) const noexcept
        {
            return m_states.at(typeIdx).get();
        }
    };
}

#endif

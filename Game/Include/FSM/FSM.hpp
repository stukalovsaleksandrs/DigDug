#ifndef GAME_FSM
#define GAME_FSM

// Engine
#include "Engine/Scene/GameObject.hpp"
// Standard
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <optional>

namespace Engine
{
    class MovementComponent;
    class AnimationComponent;
}

namespace Game
{
    class LevelManager;
#pragma region StateBase

    using StateType = std::optional<std::type_index>;
    class StateBase
    {
    public:
        struct Dependencies final
        {
            Engine::AnimationComponent& animationComponent;
            Engine::MovementComponent const& movementComponent;
            Engine::GameObject& owner;// GetWorldLocation is not const
            LevelManager const& levelManager;
        };
        explicit StateBase(Dependencies const& dependencies) noexcept
            : m_dependencies{ dependencies }{}
        virtual ~StateBase() = default;
        virtual StateType Update() noexcept = 0;
        virtual void OnEnter() noexcept = 0;
        virtual void OnExit() noexcept = 0;

    protected:
        Dependencies m_dependencies;

        [[nodiscard]] bool IsMoving() const noexcept;
    };

#pragma endregion StateBase

#pragma region FSM
    class FSM final
    {
    public:
        using States = std::unordered_map<std::type_index, std::unique_ptr<StateBase>>;

        // NOTE: Not using a struct since using one results when moving the first argument
        explicit FSM(std::pair<States&&, StateBase*> const&) noexcept;
        void Update() noexcept;

    private:
        States m_states;
        // Not a ref, bc, when changing state, assigning ref calls copy assignment
        StateBase* m_pCurrentState{};
        void TryChangingState(StateType);

        [[nodiscard]] StateBase* StatesAt(std::type_index const& typeIdx) const noexcept
        {
            return m_states.at(typeIdx).get();
        }
    };
#pragma endregion FSM
}

#endif// GAME_FSM

#ifndef GAME_FSM
#define GAME_FSM

// Game
#include "Utils.hpp"
// Engine
#include "Engine/Scene/GameObject.hpp"
// Standard
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <optional>

namespace Engine
{
    struct InputAction;
    class MovementComponent;
    class AnimationComponent;
}

namespace Game
{
    class Level;
#pragma region StateBase

    using StateType = std::optional<std::type_index>;
    class StateBase
    {
    public:
        virtual ~StateBase() = default;
        [[nodiscard]] virtual StateType Update() noexcept{ return std::nullopt; }
        [[nodiscard]] virtual StateType ProcessGameAction(GameAction) noexcept;
        virtual void OnEnter() noexcept{}
        virtual void OnExit() noexcept{}
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
        void ProcessGameAction(GameAction) noexcept;

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

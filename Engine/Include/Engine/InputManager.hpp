#ifndef ENGINE_INPUT_MANAGER
#define ENGINE_INPUT_MANAGER

// Engine
#include "Engine/Commands.hpp"
// Standard
#include <variant>
#include <utility>

#include "Core/Singleton.hpp"

namespace Engine
{
    class Command;

    enum class InputType
    {
        held,
        released,
    };

    struct Action final
    {
        std::variant<SDL_Scancode, SDL_GamepadButton> input;
        InputType type;

        bool operator==(Action const& other) const noexcept
        {
            if (type != other.type) return false;
            return std::visit([&]<typename T0, typename T1>(T0 const& lhs, T1 const& rhs) -> bool
            {
                using L = std::decay_t<T0>;
                using R = std::decay_t<T1>;
                if constexpr (std::is_same_v<L, R>)
                    return static_cast<int32_t>(lhs) == static_cast<int32_t>(rhs);
                else
                    return false;
            }, input, other.input);
        }
    };

    // TODO: Use dependency injection instead of singleton
    class InputManager final : public Singleton<InputManager>
    {
    public:
        InputManager();
        ///@return Whether the application has to quit
        [[nodiscard]] bool ProcessInput();
        void Bind(Action const&, std::unique_ptr<Command>);
        void Unbind(Action const&);

    private:
        struct ActionHash
        {
            size_t operator()(Action const& action) const noexcept
            {
                std::size_t const h1 = std::hash<std::variant<SDL_Scancode, SDL_GamepadButton>>{}(action.input);
                std::size_t const h2 = std::hash<std::underlying_type_t<InputType>>{}(std::to_underlying(action.type));
                return h1 ^ h2 << 1;
            }
        };
        std::unordered_map<Action, std::unique_ptr<Command>, ActionHash> m_actionToCommand;

        void ExecuteIfExists(Action const& action) const;
        void ProcessPressing();
        void InitializeGamepad();
    };

}

#endif// ENGINE_INPUT_MANAGER

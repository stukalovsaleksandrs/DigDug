#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Commands.h"
#include "Engine/Core/Singleton.h"
#include <variant>
#include <utility>

namespace DAE::Input
{
    class Command;

    // Button or key
    using Input = std::variant<SDL_Scancode, SDL_GamepadButton>;

    // Physical action on the button or key
    enum class InputMode : uint8_t
    {
        held,
        released,
    };

    // Combination of button/key and physical action on it
    // using Action = std::pair<Input, InputMode>;
    struct Action final
    {
        Input input;
        InputMode mode;

        // NOTE: Required by std::unordered_map
        bool operator==(Action const& other) const noexcept
        {
            return input == other.input && mode == other.mode;
        }
    };

    // Combination of action and command with automatic binding to InputManager
    class Binding final
    {
    public:
        Binding(Action action, std::unique_ptr<Command> pCommand) noexcept;
        ~Binding();
        Binding(Binding const&) noexcept = delete;
        // NOTE: Self assignment is impossible, it's a constructor.
        // Also the previous action does not need to get unbound,
        // because it was not bound to begin with.
        // Action is trivially copyable.
        Binding(Binding&&) noexcept = default;
        Binding& operator=(Binding const&) noexcept = delete;
        Binding& operator=(Binding&&) noexcept = delete;

    private:
        Action m_action;

    };

    class InputManager final : public Singleton<InputManager>
    {
    public:
        InputManager();
        ///@return Whether the application has to quit
        [[nodiscard]] bool ProcessInput();
        void Bind(Action, std::unique_ptr<Command>);
        void Unbind(Action);

    private:
        struct ActionHash final
        {
            size_t operator()(Action const& action) const noexcept
            {
                std::size_t const h1 = std::hash<std::variant<SDL_Scancode, SDL_GamepadButton>>{}(action.input);
                std::size_t const h2 = std::hash<std::underlying_type_t<InputMode>>{}(std::to_underlying(action.mode));
                return h1 ^ h2 << 1;
            }
        };
        // NOTE: Right now, only one button can be bound to
        std::unordered_map<Action, std::unique_ptr<Command>, ActionHash> m_actionToCommand;

        void ExecuteIfExists(Action const& action) const;
        void ProcessHolding();
        void InitializeGamepad();
    };

}

#endif

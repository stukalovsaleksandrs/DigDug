#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "Commands.h"
#include "Singleton.h"
#include <variant>

namespace DAE::Input
{
    class Command;
    // An action is a scancode(e.g., button, key, gamepad stick) and a state(e.g., pressed or held)
    using Action = std::pair<std::variant<SDL_Scancode, SDL_GamepadButton>, Uint32>;

    class InputManager final : public Singleton<InputManager>
    {
    public:
        ///@return Whether the application has to quit
        [[nodiscard]] bool ProcessInput() const;
        void Bind(Action const&, std::unique_ptr<Command>);

    private:
        struct ActionHash
        {
            size_t operator()(Action const& action) const noexcept
            {
                std::size_t const h1 = std::hash<std::variant<SDL_Scancode, SDL_GamepadButton>>{}(action.first);
                std::size_t const h2 = std::hash<Uint32>{}(action.second);
                return h1 ^ h2 << 1;
            }
        };
        std::unordered_map<Action, std::unique_ptr<Command>, ActionHash> m_inputToCommand;

    };

}

#endif

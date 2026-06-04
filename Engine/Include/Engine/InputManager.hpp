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

    using Action = std::pair<std::variant<SDL_Scancode, SDL_GamepadButton>, InputType>;

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
                std::size_t const h1 = std::hash<std::variant<SDL_Scancode, SDL_GamepadButton>>{}(action.first);
                std::size_t const h2 = std::hash<std::underlying_type_t<InputType>>{}(std::to_underlying(action.second));
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

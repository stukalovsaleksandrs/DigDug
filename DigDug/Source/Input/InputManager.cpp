#include "Input/InputManager.h"
#include <ranges>
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
#include <print>

DAE::Input::InputManager::InputManager()
{
    InitializeGamepad();
}

bool DAE::Input::InputManager::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return false;
        }

        switch (event.type)
        {
        case SDL_EVENT_KEY_UP:
            ExecuteIfExists({event.key.scancode, InputType::released});
            break;
        default:;
        }

        // Processing ImGui events
        ImGui_ImplSDL3_ProcessEvent(&event);
    }

    ProcessPressing();

    return true;
}

void DAE::Input::InputManager::Bind(Action const& action, std::unique_ptr<Command> pCommand)
{
    m_actionToCommand[action] = std::move(pCommand);
}

void DAE::Input::InputManager::Unbind(Action const& action)
{
    m_actionToCommand.erase(action);
}

void DAE::Input::InputManager::ProcessPressing()
{
    auto const keyboardState{ SDL_GetKeyboardState(nullptr) };
    for (const auto& key : m_actionToCommand | std::views::keys)
    {
        // Returning early if the action is not bound to the pressed type
        auto const & action{ key };
        if (std::get<InputType>(action) != InputType::pressed) continue;

        // Returning early if the key is not pressed
        if (!keyboardState[std::get<SDL_Scancode>(action.first)]) continue;

        // Bound to pressed & actually pressed -> executing
        ExecuteIfExists(action);
    }
}

void DAE::Input::InputManager::ExecuteIfExists(Action const& action) const
{
    if (m_actionToCommand.contains(action))
    {
        m_actionToCommand.at(action)->Execute();
    }
}

void DAE::Input::InputManager::InitializeGamepad()
{
    // Early returning if no gamepad
    if (!SDL_HasGamepad())
    {
        std::println("No gamepads connected");
        return;
    }

    // Getting the first connected gamepad
    // int gamepadCount{};
    // SDL_JoystickID* gamepads{ SDL_GetGamepads(&gamepadCount) };
    // TODO: Finish gamepad initialization logic
}

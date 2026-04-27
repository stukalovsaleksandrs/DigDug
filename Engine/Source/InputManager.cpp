// Engine
#include "InputManager.h"
// Third-party
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
// Standard
#include <ranges>
#include <print>

Engine::InputManager::InputManager()
{
    InitializeGamepad();
}

bool Engine::InputManager::ProcessInput()
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

void Engine::InputManager::Bind(Action const& action, std::unique_ptr<Command> pCommand)
{
    m_actionToCommand[action] = std::move(pCommand);
}

void Engine::InputManager::Unbind(Action const& action)
{
    m_actionToCommand.erase(action);
}

void Engine::InputManager::ProcessPressing()
{
    auto const keyboardState{ SDL_GetKeyboardState(nullptr) };
    for (const auto& key : m_actionToCommand | std::views::keys)
    {
        // Returning early if the action is not bound to the held type
        auto const & action{ key };
        if (std::get<InputType>(action) != InputType::held) continue;

        // Returning early if the key is not held
        if (!keyboardState[std::get<SDL_Scancode>(action.first)]) continue;

        // Bound to held & actually held -> executing
        ExecuteIfExists(action);
    }
}

void Engine::InputManager::ExecuteIfExists(Action const& action) const
{
    if (m_actionToCommand.contains(action))
    {
        m_actionToCommand.at(action)->Execute();
    }
}

void Engine::InputManager::InitializeGamepad()
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

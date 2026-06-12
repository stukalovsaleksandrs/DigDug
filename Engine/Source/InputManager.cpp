// Engine
#include "InputManager.hpp"
// Third-party
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>
// Standard
#include <ranges>
#include <print>

SDL_Gamepad* g_pGamepad{};

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

void Engine::InputManager::Bind(InputAction const& action, std::unique_ptr<Command> pCommand)
{
    m_actionToCommand[action] = std::move(pCommand);
}

void Engine::InputManager::Unbind(InputAction const& action)
{
    m_actionToCommand.erase(action);
}

void Engine::InputManager::ProcessPressing()
{
    auto const keyboardState{ SDL_GetKeyboardState(nullptr) };
    for (const auto& action : m_actionToCommand | std::views::keys)
    {
        // Returning early if the action is not bound to the held type
        if (action.type != InputType::held) continue;

        // Returning early if the key is not held
        if (auto* pKey{ std::get_if<SDL_Scancode>(&action.input) })
        {
            if (not keyboardState[*pKey]) continue;
        }
        else if (auto* pGamepadButton{ std::get_if<SDL_GamepadButton>(&action.input) })
        {
            if (not SDL_HasGamepad()) continue;
            if (not SDL_GetGamepadButton(g_pGamepad, *pGamepadButton)) continue;
        }

        // Bound to held & actually held -> executing
        ExecuteIfExists(action);
    }
}

void Engine::InputManager::ExecuteIfExists(InputAction const& action) const
{
    if (m_actionToCommand.contains(action))
    {
        m_actionToCommand.at(action)->Execute();
    }
}

void Engine::InputManager::InitializeGamepad()
{
    // Early returning if no gamepad
    if (not SDL_HasGamepad())
    {
        std::println("No gamepads connected");
        return;
    }

    // Getting the first connected gamepad
    // Source: https://glusoft.com/sdl3-tutorials/use-gamepads-joysticks-sdl3
    int32_t deviceCount{};
    SDL_JoystickID const* const devices{ SDL_GetGamepads(&deviceCount) };
    for (int32_t const deviceIdx : std::ranges::views::iota(0, deviceCount))
    {
        SDL_Gamepad* pGamepad{ SDL_OpenGamepad(devices[deviceIdx]) };
        if (not g_pGamepad)
        {
            g_pGamepad = pGamepad;
        }

        std::println("Gamepad connected: {}", SDL_GetGamepadName(pGamepad));

        if (deviceIdx > 0) SDL_CloseGamepad(pGamepad);
    }
}

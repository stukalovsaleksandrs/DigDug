#include "Input/InputManager.h"
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>

bool DAE::Input::InputManager::ProcessInput() const
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return false;
        }

        if (Action const action{event.key.scancode, event.type};
            m_inputToCommand.contains(action))
        {
            m_inputToCommand.at(action)->Execute();
        }

        // Processing ImGui events
        ImGui_ImplSDL3_ProcessEvent(&event);
    }

    return true;
}

void DAE::Input::InputManager::Bind(Action const& action, std::unique_ptr<Command> pCommand)
{
    m_inputToCommand[action] = std::move(pCommand);
}

#include "Input/InputManager.h"
#include <SDL3/SDL.h>
#include <backends/imgui_impl_sdl3.h>

bool DAE::Input::InputManager::ProcessInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return false;
        }

        // Processing ImGui events
        ImGui_ImplSDL3_ProcessEvent(&event);
    }

    return true;
}

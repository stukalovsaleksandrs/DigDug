// Project
#include "Application.h"
// Engine
#include "Engine/Sound/SoundServiceLocator.h"
#include "Engine/Utils/Utils.h"
// Third-party
#include "SDL3/SDL_main.h"// Required for the windows build not to give errors
#include "SDL3/SDL.h"
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

int main(int, char*[]) {
    Game::Application game{};
    game.Run();
    return SDL_APP_SUCCESS;
}

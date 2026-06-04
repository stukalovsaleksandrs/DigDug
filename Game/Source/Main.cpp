// Project
#include "Application.hpp"
// Third-party
#include "SDL3/SDL_main.h"// Required for the windows build not to give errors
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

int main(int, char*[]) {
    Game::Application game{};
    game.Run();
}

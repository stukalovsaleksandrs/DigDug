// Project
#include "Application.h"
// Third-party
#include "Engine/Utils/Utils.h"
#include "SDL3/SDL_main.h"// Required for the windows build not to give errors
#include "SDL3/SDL.h"
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "SDL3_mixer/SDL_mixer.h"
static MIX_Mixer *mixer;
static MIX_Track *track;
static MIX_Audio *audio;

void PlaySound()
{
    //Initialize SDL_mixer
    if (!MIX_Init())
    {
        Engine::Utils::ThrowSDLError("MIX_Init failed");
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer)
    {
        Engine::Utils::ThrowSDLError("Mixer creation failed");
    }

    std::string const file{ "Resources/GameStart.mp3" };

    audio = MIX_LoadAudio(mixer, file.data(), true);
    if (!audio)
    {
        Engine::Utils::ThrowSDLError(std::format("Filed to load {}", file));
    }

    track = MIX_CreateTrack(mixer);
    if (!track)
    {
        Engine::Utils::ThrowSDLError("Failed to create track");
    }

    MIX_SetTrackAudio(track, audio);
    MIX_PlayTrack(track, 0);
}

int main(int, char*[]) {

    Game::Application game{};
    PlaySound();
    game.Run();

    return SDL_APP_SUCCESS;
    MIX_Quit();// <- Don't forget that
}

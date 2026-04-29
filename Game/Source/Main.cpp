// Project
#include "Application.h"
// Third-party
#include "Engine/Sound/SoundServiceLocator.h"
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
    auto& soundService{ Engine::SoundServiceLocator::GetSoundService() };
    soundService.PlaySound(1);

    Engine::Utils::Check(MIX_Init(), "MIX_Init failed");

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    Engine::Utils::Check(mixer,"Mixer creation failed");

    std::string const file{ "Resources/GameStart.mp3" };
    audio = MIX_LoadAudio(mixer, file.data(), true);
    Engine::Utils::Check(audio, std::format("Filed to load {}", file));

    track = MIX_CreateTrack(mixer);
    Engine::Utils::Check(track,"Failed to create track");

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

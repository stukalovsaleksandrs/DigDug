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

#include "SDL3_mixer/SDL_mixer.h"

int main(int, char*[]) {
    Game::Application game{};
    {
        auto mixerDeleter{ [](MIX_Mixer* pMixer)
        {
            MIX_DestroyMixer(pMixer);
        } };
        std::unique_ptr<MIX_Mixer, decltype(mixerDeleter)> pMixer{ nullptr, mixerDeleter};
        // MIX_Mixer *pMixer;
        MIX_Audio *audio;
        MIX_Track *track;

        auto& soundService{ Engine::SoundServiceLocator::GetSoundService() };
        soundService.PlaySound(1);

        Engine::Utils::Check(MIX_Init(), "MIX_Init failed");

        pMixer.reset(MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL));
        // pMixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
        Engine::Utils::Check(pMixer.get(),"Mixer creation failed");
        // Engine::Utils::Check(pMixer,"Mixer creation failed");

        std::string const file{ "Resources/GameStart.mp3" };
        audio = MIX_LoadAudio(pMixer.get(), file.data(), true);
        // audio = MIX_LoadAudio(pMixer, file.data(), true);
        Engine::Utils::Check(audio, std::format("Filed to load {}", file));

        track = MIX_CreateTrack(pMixer.get());
        // track = MIX_CreateTrack(pMixer);
        Engine::Utils::Check(track,"Failed to create track");

        MIX_SetTrackAudio(track, audio);
        MIX_PlayTrack(track, 0);

        game.Run();

        // MIX_StopTrack(track, 0);
        MIX_DestroyAudio(audio);
        // Will get destroyed automatically together with the mixer
        MIX_DestroyTrack(track);
        // pMixer.reset();
        // MIX_DestroyMixer(pMixer);
    }
    MIX_Quit();
    return SDL_APP_SUCCESS;
}

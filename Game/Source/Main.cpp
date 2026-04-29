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

        auto trackDeleter{ [](MIX_Track* pTrack)
        {
            MIX_StopTrack(pTrack, 10);
            MIX_DestroyTrack(pTrack);
        } };
        std::unique_ptr<MIX_Track, decltype(trackDeleter)> pTrack{ nullptr, trackDeleter};

        auto audioDeleter{ [](MIX_Audio* pAudio)
        {
            MIX_DestroyAudio(pAudio);
        } };
        std::unique_ptr<MIX_Audio, decltype(audioDeleter)> pAudio{ nullptr, audioDeleter };

        auto& soundService{ Engine::SoundServiceLocator::GetSoundService() };
        soundService.PlaySound(1);

        Engine::Utils::Check(MIX_Init(), "MIX_Init failed");

        pMixer.reset(MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL));
        Engine::Utils::Check(pMixer.get(),"Mixer creation failed");

        std::string const file{ "Resources/GameStart.mp3" };
        pAudio.reset(MIX_LoadAudio(pMixer.get(), file.data(), true));
        Engine::Utils::Check(pAudio.get(), std::format("Filed to load {}", file));

        pTrack.reset(MIX_CreateTrack(pMixer.get()));
        Engine::Utils::Check(pTrack.get(),"Failed to create track");

        MIX_SetTrackAudio(pTrack.get(), pAudio.get());
        MIX_PlayTrack(pTrack.get(), 0);

        game.Run();
    }
    MIX_Quit();
    return SDL_APP_SUCCESS;
}

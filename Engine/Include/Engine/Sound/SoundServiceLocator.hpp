#ifndef ENGINE_SOUND
#define ENGINE_SOUND

// Engine
#include "ISoundService.hpp"


// A globally-accessible singleton of a service locator used to
// acquire a reference to the currently-active sound system
namespace Engine::SoundServiceLocator
{
    ISoundService& GetSoundService() noexcept;
    void SetSoundService(ISoundService&) noexcept;
}


#endif// ENGINE_SOUND

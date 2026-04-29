#ifndef ENGINE_SOUND_H
#define ENGINE_SOUND_H

// Engine
#include "ISoundService.h"

namespace Engine
{
    // A globally-accessible singleton of a service locator used to
    // acquire a reference to the currently-active sound system
    namespace SoundServiceLocator
    {
        ISoundService& GetSoundService() noexcept;
        void SetSoundService(ISoundService&) noexcept;
    }
}

#endif// ENGINE_SOUND_H

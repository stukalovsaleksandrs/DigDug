#ifndef ENGINE_CONSTANTS
#define ENGINE_CONSTANTS

// Engine
#include "Engine/Core/Observer.h"
// Standard
#include <cstdint>

namespace Engine
{
    enum class EventType : EventId
    {
        OnDirectionChanged = MakeSDBMHash("OnDirectionChanged")
    };
}

#endif// ENGINE_CONSTANTS

#ifndef ENGINE_CONSTANTS
#define ENGINE_CONSTANTS

// Engine
#include "Utils.h"
// Standard
#include <cstdint>

namespace Engine
{
    using EventId = uint32_t;
    enum class EventType : EventId
    {
        SubjectDeleted = Utils::MakeSDBMHash("SubjectDeleted"),
        OnDirectionChanged = Utils::MakeSDBMHash("OnDirectionChanged"),
        OnMovementAxisChanged = Utils::MakeSDBMHash("OnMovementAxisChanged"),
    };
}

#endif// ENGINE_CONSTANTS

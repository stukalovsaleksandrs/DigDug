#ifndef ENGINE_CONSTANTS
#define ENGINE_CONSTANTS

// Engine
#include "Utils.hpp"
// Standard
#include <cstdint>

namespace Engine
{
    using EventId = uint32_t;
    enum class EventType : EventId
    {
        SubjectDeleted = "SubjectDeleted"_h,
        OnDirectionChanged = "OnDirectionChanged"_h,
        OnMovementAxisChanged = "OnMovementAxisChanged"_h,
    };
}

#endif// ENGINE_CONSTANTS

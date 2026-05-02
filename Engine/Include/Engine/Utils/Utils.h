#ifndef SE_UTILS_H
#define SE_UTILS_H

// Third-party
#include "glm/vec2.hpp"
// Standard
#include <string_view>

namespace Engine::Utils {
    struct Bounds final
    {
        glm::vec2 topLeft, dims;
    };

    void Check(bool result, std::string_view message);

    void ThrowSDLError(std::string_view message);

    void LogSDLVersion(std::string_view message, int major, int minor, int patch);

    void PrintSDLVersion();

    bool NearlyEqual(glm::vec2 lhs, glm::vec2 rhs);

}

#endif// SE_UTILS_H

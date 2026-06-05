#ifndef ENGINE_UTILS
#define ENGINE_UTILS

// Third-party
#include "glm/vec2.hpp"
// Standard
#include <memory>
#include <ranges>
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

    /********************
     * Comparisons
     ********************/
    bool NearlyEqual(glm::vec2 lhs, glm::vec2 rhs);
    bool NearlyEqual(float lhs, float rhs);
    bool NearlyZero(float value);

    template<typename T, typename U>
    bool HaveSameType(const std::unique_ptr<T>&, const std::unique_ptr<U>&) {
        return std::is_same_v<T, U>;
    }
}

/********************
 * SDBM hashing
 ********************/
consteval unsigned int MakeSDBMHash(char const* str, size_t len)
{
    unsigned int hash{};
    for (uint32_t const charIdx : std::ranges::views::iota(0u, len))
    {
        hash = static_cast<uint32_t>(str[charIdx]) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

consteval unsigned int operator ""_h(const char * str, size_t len)
{
    return MakeSDBMHash(str, len);
}

#endif// ENGINE_UTILS

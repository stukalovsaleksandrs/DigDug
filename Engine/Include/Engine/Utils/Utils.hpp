#ifndef ENGINE_UTILS
#define ENGINE_UTILS

// Third-party
#include "glm/vec2.hpp"
#include <glm/ext/scalar_constants.hpp>
// Standard
#include <memory>
#include <ranges>
#include <string_view>
#include <sstream>

namespace Engine::Utils {
    struct Bounds final
    {
        glm::vec2 topLeft, dims;
    };

    struct Circle final
    {
        glm::vec2 center;
        float radius;
    };

    struct Square final
    {
        glm::vec2 topLeft;
        float sideLength;

        struct Corners
        {
            bool topLeft     : 1 { true };
            bool topRight    : 1 { true };
            bool bottomLeft  : 1 { true };
            bool bottomRight : 1 { true };
        } roundedCorners{};

        float cornerRadius{ 5.f };
    };

    void Check(bool result, std::string_view message);

    void ThrowSDLError(std::string_view message);

    void LogSDLVersion(std::string_view message, int major, int minor, int patch);

    void PrintSDLVersion();

#pragma region Comparors
    bool NearlyEqual(glm::vec2 lhs, glm::vec2 rhs, float epsilon = glm::epsilon<float>());
    bool NearlyEqual(float lhs, float rhs, float epsilon = glm::epsilon<float>());
    bool NearlyZero(float value, float epsilon = glm::epsilon<float>());
    bool NearlyZero(glm::vec2 const& value, float epsilon);

    template<typename T, typename U>
    bool HaveSameType(const std::unique_ptr<T>&, const std::unique_ptr<U>&) {
        return std::is_same_v<T, U>;
    }

    struct StrictWeakComparor_i32vec2
    {
        bool operator()(glm::i32vec2 const lhs, glm::i32vec2 const rhs) const
        {
            if (lhs.x != rhs.x) return lhs.x < rhs.x;
            return lhs.y < rhs.y;
        }
    };

    struct Hash_i32vec2
    {
        bool operator()(glm::i32vec2 const v) const
        {
            return std::hash<uint32_t>()(v.x) ^ std::hash<uint32_t>()(v.y);
        }
    };

#pragma endregion Comparors
}

#pragma region SDBM
consteval unsigned int MakeSDBMHash(char const* str, size_t len)
{
    unsigned int hash{};
    for (uint32_t const charIdx : std::ranges::views::iota(0u, len))
    {
        hash = static_cast<uint32_t>(str[charIdx]) + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

consteval unsigned int operator ""_h(const char * str, size_t const len)
{
    return MakeSDBMHash(str, len);
}
#pragma endregion SDBM

#endif// ENGINE_UTILS

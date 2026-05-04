#ifndef SE_UTILS_H
#define SE_UTILS_H

// Third-party
#include "glm/vec2.hpp"
// Standard
#include <memory>
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

    /********************
     * SDBM hashing
     ********************/
    template <int length> struct SDBMHash
    {
        consteval static unsigned int _Calculate(const char* const text, unsigned int& value) {
            const unsigned int character = SDBMHash<length - 1>::_Calculate(text, value);
            value = character + (value << 6) + (value << 16) - value;
            return text[length - 1];
        }
        consteval static unsigned int Calculate(const char* const text) {
            unsigned int value = 0;
            const auto character = _Calculate(text, value);
            return character + (value << 6) + (value << 16) - value;
        }
    };
    template <> struct SDBMHash<1> {
        consteval static int _Calculate(const char* const text, unsigned int& ) { return text[0]; }
    };
    template <size_t N> consteval unsigned int MakeSDBMHash(const char (&text)[N]) {
        return SDBMHash<N - 1>::Calculate(text);
    }



}

#endif// SE_UTILS_H

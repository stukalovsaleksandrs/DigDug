#ifndef LEVEL_PARSER
#define LEVEL_PARSER
// Standard
#include <string_view>
#include <vector>

namespace Game
{
    class LevelManager final
    {
    public:
        explicit LevelManager(std::vector<std::string_view> paths) noexcept;

    private:
        struct Level final
        {

        };

    };
}

#endif// LEVEL_PARSER

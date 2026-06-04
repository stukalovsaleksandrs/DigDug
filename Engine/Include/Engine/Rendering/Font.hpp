#ifndef ENGINE_FONT
#define ENGINE_FONT

// Standard
#include <string_view>

struct TTF_Font;
namespace Engine
{
    /**
     * Simple RAII wrapper for a TTF_Font
     */
    class Font final
    {
    public:
        [[nodiscard]] TTF_Font* GetFont() const;
        explicit Font(std::string_view fullPath, float size);
        ~Font();

        Font(Font const&) = delete;
        Font(Font &&) = delete;
        Font & operator= (Font const&) = delete;
        Font & operator= (Font const&&) = delete;

    private:
        TTF_Font* m_font;

    };
}

#endif// ENGINE_FONT

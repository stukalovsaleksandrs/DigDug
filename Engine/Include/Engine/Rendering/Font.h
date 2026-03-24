#ifndef FONT_H
#define FONT_H
#include <string_view>

struct TTF_Font;
namespace DAE
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

#endif

#include "Font.h"
#include "Utils.h"
#include <SDL3_ttf/SDL_ttf.h>

TTF_Font* DAE::Font::GetFont() const {
    return m_font;
}

DAE::Font::Font(std::string_view const fullPath, float const size)
    : m_font()
{
    m_font = TTF_OpenFont(fullPath.data(), size);
    if (!m_font)
    {
        Utils::ThrowSDLError("Failed to load font");
    }
}

DAE::Font::~Font()
{
    TTF_CloseFont(m_font);
}

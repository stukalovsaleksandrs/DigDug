#include "Rendering/Font.h"
#include "Utils/Utils.h"
#include <SDL3_ttf/SDL_ttf.h>

TTF_Font* Engine::Font::GetFont() const {
    return m_font;
}

Engine::Font::Font(std::string_view const fullPath, float const size)
    : m_font()
{
    m_font = TTF_OpenFont(fullPath.data(), size);
    if (!m_font)
    {
        Utils::ThrowSDLError("Failed to load font");
    }
}

Engine::Font::~Font()
{
    TTF_CloseFont(m_font);
}

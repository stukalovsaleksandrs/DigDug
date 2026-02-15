#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Utils.h"

DAE::TextObject::TextObject(std::string_view const text, std::shared_ptr<Font> font, SDL_Color const& color)
    : m_text(text), m_color(color), m_font(std::move(font))
{}

void DAE::TextObject::Update()
{
    if (m_needsUpdate)
    {
        auto const pSurface{ TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_text.length(), m_color) };
        if (!pSurface)
        {
            Utils::ThrowSDLError("Render text failed");
        }
        auto pTexture{ SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), pSurface) };
        if (!pTexture)
        {
            Utils::ThrowSDLError("Create text texture from surface failed");
        }
        SDL_DestroySurface(pSurface);
        m_textTexture = std::make_shared<Texture2D>(pTexture);
        m_needsUpdate = false;
    }
}

void DAE::TextObject::Render() const
{
    if (m_textTexture)
    {
        auto const& location{ m_transform.GetLocation() };
        Renderer::GetInstance().RenderTexture(*m_textTexture, location);
    }
}

void DAE::TextObject::SetText(std::string_view const text)
{
    m_text = text;
    m_needsUpdate = true;
}

void DAE::TextObject::SetLocation(glm::vec2 const location)
{
    m_transform.SetLocation({location.x, location.y, 0.f});
}

void DAE::TextObject::SetColor(SDL_Color const& color)
{ 
    m_color = color;
    m_needsUpdate = true;
}

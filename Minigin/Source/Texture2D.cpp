#include "Texture2D.h"
#include "Renderer.h"
#include "Utils.h"
#include <SDL3/SDL.h>
#include <format>

DAE::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

glm::vec2 DAE::Texture2D::GetSize() const
{
    glm::vec2 size{};
    SDL_GetTextureSize(m_pTexture, &size.x, &size.y);
    return size;
}

SDL_Texture* DAE::Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

DAE::Texture2D::Texture2D(std::string_view const fullPath)
{
    SDL_Surface* pSurface{ SDL_LoadPNG(fullPath.data()) };
    if (!pSurface)
    {
        Utils::ThrowSDLError("Failed to load PNG");
    }

    m_pTexture = SDL_CreateTextureFromSurface(
        Renderer::GetInstance().GetSDLRenderer(),
        pSurface
    );

    SDL_DestroySurface(pSurface);

    if (!m_pTexture)
    {
        Utils::ThrowSDLError("Failed to create texture from surface");
    }
}

DAE::Texture2D::Texture2D(SDL_Texture* pTexture)
    : m_pTexture{ pTexture }
{
	assert(m_pTexture);
}


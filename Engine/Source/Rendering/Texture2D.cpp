#include "Rendering/Texture2D.h"
#include "Rendering/Renderer.h"
#include "Utils/Utils.h"
#include <SDL3/SDL.h>
#include <format>

Engine::Texture2D::~Texture2D()
{
    SDL_DestroyTexture(m_pTexture);
}

glm::vec2 Engine::Texture2D::GetDims() const
{
    glm::vec2 dims{};
    SDL_GetTextureSize(m_pTexture, &dims.x, &dims.y);
    return dims;
}

SDL_Texture* Engine::Texture2D::GetSDLTexture() const
{
    return m_pTexture;
}

Engine::Texture2D::Texture2D(std::string_view const fullPath)
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

Engine::Texture2D::Texture2D(SDL_Texture* pTexture)
    : m_pTexture{ pTexture }
{
    assert(m_pTexture);
}


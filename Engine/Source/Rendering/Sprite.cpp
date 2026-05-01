// Engine
#include "Utils/Utils.h"
#include "Rendering/Sprite.h"
#include "Rendering/Renderer.h"
// Third-party
#include <SDL3/SDL.h>
// Standard
#include <format>

Engine::Sprite::~Sprite()
{
    SDL_DestroyTexture(m_pTexture);
}

glm::ivec2 Engine::Sprite::GetDims() const
{
    glm::vec2 dims{};
    SDL_GetTextureSize(m_pTexture, &dims.x, &dims.y);
    return {static_cast<int>(dims.x), static_cast<int>(dims.y)};
}

SDL_Texture* Engine::Sprite::GetSDLTexture() const
{
    return m_pTexture;
}

Engine::Sprite::Sprite(std::string_view const fullPath)
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

Engine::Sprite::Sprite(SDL_Texture* pTexture)
    : m_pTexture{ pTexture }
{
    assert(m_pTexture);
}


// Engine
#include "Utils/Utils.hpp"
#include "Rendering/Sprite.hpp"
#include "Rendering/Renderer.hpp"
// Third-party
#include <SDL3/SDL.h>
// Standard
#include <format>

Engine::Sprite::~Sprite() noexcept
{
    SDL_DestroyTexture(m_pSDLTexture);
}

glm::ivec2 Engine::Sprite::GetDims() const
{
    glm::vec2 dims{};
    SDL_GetTextureSize(m_pSDLTexture, &dims.x, &dims.y);
    return {static_cast<int>(dims.x), static_cast<int>(dims.y)};
}

SDL_FRect Engine::Sprite::GetFullBounds() const
{
    return {
        .x = 0.f, .y = 0.f,
        .w = static_cast<float>(m_pSDLTexture->w),
        .h = static_cast<float>(m_pSDLTexture->h)
    };
}

void Engine::Sprite::Render() const noexcept
{
    SDL_SetTextureBlendMode(m_pSDLTexture, m_blendMode);
    SDL_RenderTexture(m_pSDLRenderer, m_pSDLTexture, nullptr, nullptr);
}

void Engine::Sprite::MaskCircle(Utils::Circle const& circle) const noexcept
{
    SDL_SetRenderDrawColor(m_pSDLRenderer, 0, 0, 0, 255);
    SDL_SetRenderTarget(m_pSDLRenderer, m_pSDLTexture);
    Renderer::GetInstance().RenderFilledCircle(circle);
    SDL_SetRenderTarget(m_pSDLRenderer, nullptr);
}

void Engine::Sprite::MaskSquare(Utils::Square const& square) const noexcept
{
    SDL_SetRenderDrawColor(m_pSDLRenderer, 0, 0, 0, 255);
    SDL_SetRenderTarget(m_pSDLRenderer, m_pSDLTexture);
    Renderer::GetInstance().RenderFilledSquare(square);
    SDL_SetRenderTarget(m_pSDLRenderer, nullptr);
}

SDL_Texture* Engine::Sprite::GetSDLTexture() const
{
    return m_pSDLTexture;
}

Engine::Sprite::Sprite(std::string_view const fullPath)
{
    SDL_Surface* pSurface{ SDL_LoadPNG(fullPath.data()) };
    if (!pSurface)
    {
        Utils::ThrowSDLError(std::format("Failed to load {}", fullPath.data()));
    }

    m_pSDLTexture = SDL_CreateTextureFromSurface(
        Renderer::GetInstance().GetSDLRenderer(),
        pSurface
    );

    SDL_DestroySurface(pSurface);

    if (!m_pSDLTexture)
    {
        Utils::ThrowSDLError("Failed to create texture from surface");
    }
}

Engine::Sprite::Sprite(SDL_Color const& color, glm::uvec2 const dims, SDL_BlendMode const blendMode)
    : m_pSDLTexture{ SDL_CreateTexture(m_pSDLRenderer,
                                SDL_PIXELFORMAT_RGBA8888,
                               SDL_TEXTUREACCESS_TARGET,
                               dims.x, dims.y) }
    , m_blendMode{ blendMode }
{
    // Filling the texture in with color
    SDL_SetRenderTarget(m_pSDLRenderer, m_pSDLTexture);
    SDL_SetRenderDrawColor(m_pSDLRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_pSDLRenderer);
    SDL_SetRenderTarget(m_pSDLRenderer, nullptr);
}

Engine::Sprite::Sprite(SDL_Texture* pTexture)
    : m_pSDLTexture{ pTexture }
{
    assert(m_pSDLTexture);
}


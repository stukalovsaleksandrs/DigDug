#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "Utils.h"
#include "Components.h"

void DAE::Renderer::Init(SDL_Window* pWindow)
{
    m_pWindow = pWindow;

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

#if defined(__EMSCRIPTEN__)
    m_pRenderer = SDL_CreateRenderer(pWindow, nullptr);
#else
    m_pRenderer = SDL_CreateRenderer(pWindow, nullptr);
#endif

    if (!m_pRenderer)
    {
        Utils::ThrowSDLError("SDL_CreateRenderer Error");
    }
}

void DAE::Renderer::Render() const
{
    // Clearing the background
    const auto&[r, g, b, a]{ GetBackgroundColor() };
    SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a);
    SDL_RenderClear(m_pRenderer);

    // Rendering the render components
    for (auto const renderComponent : m_pRenderComponents) {
        renderComponent->Render();
    }

    // Presenting the surface
    SDL_RenderPresent(m_pRenderer);
}

void DAE::Renderer::Destroy()
{
    if (m_pRenderer)
    {
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = nullptr;
    }
}

void DAE::Renderer::RenderTexture(Texture2D const& texture, glm::vec2 const location) const
{
    SDL_FRect destination{};
    destination.x = location.x;
    destination.y = location.y;
    SDL_GetTextureSize(texture.GetSDLTexture(), &destination.w, &destination.h);
    SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &destination);
}

void DAE::Renderer::RenderTexture(Texture2D const& texture, glm::vec2 const location, glm::vec2 const dimensions) const
{
    SDL_FRect destination{};
    destination.x = location.x;
    destination.y = location.y;
    destination.w = dimensions.x;
    destination.h = dimensions.y;
    SDL_RenderTexture(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &destination);
}

SDL_Renderer* DAE::Renderer::GetSDLRenderer() const { return m_pRenderer; }

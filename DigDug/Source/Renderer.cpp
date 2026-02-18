#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "Utils.h"

void DAE::Renderer::Init(SDL_Window* window)
{
    m_window = window;

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

#if defined(__EMSCRIPTEN__)
    m_renderer = SDL_CreateRenderer(window, nullptr);
#else
    m_renderer = SDL_CreateRenderer(window, nullptr);
#endif

    if (!m_renderer)
    {
        Utils::ThrowSDLError("SDL_CreateRenderer Error");
    }
}

void DAE::Renderer::Render() const
{
    auto const& color{ GetBackgroundColor() };
    SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(m_renderer);

    SceneManager::GetInstance().Render();

    SDL_RenderPresent(m_renderer);
}

void DAE::Renderer::Destroy()
{
    if (m_renderer)
    {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
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

SDL_Renderer* DAE::Renderer::GetSDLRenderer() const { return m_renderer; }

#ifndef RENDERER_H
#define RENDERER_H
#include "Singleton.h"
#include <SDL3/SDL.h>
#include <glm/vec2.hpp>

namespace DAE
{
    class Texture2D;
    /**
     * Simple RAII wrapper for the SDL renderer
     */
    class Renderer final : public Singleton<Renderer>
    {
    public:
        void Init(SDL_Window* window);
        void Render() const;
        void Destroy();

        void RenderTexture(Texture2D const& texture, glm::vec2 location) const;
        void RenderTexture(Texture2D const& texture, glm::vec2 location, glm::vec2 dimensions) const;

        SDL_Renderer* GetSDLRenderer() const;

        const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
        void SetBackgroundColor(SDL_Color const& color) { m_clearColor = color; }

    private:
        SDL_Renderer* m_renderer{};
        SDL_Window* m_window{};
        SDL_Color m_clearColor{};

    };
}

#endif

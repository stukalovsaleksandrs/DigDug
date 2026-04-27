#ifndef SE_RENDERER_H
#define SE_RENDERER_H

// Engine
#include "Engine/Core/Singleton.h"
// Third-party
#include <SDL3/SDL.h>
#include <glm/vec2.hpp>
// Standard
#include <vector>
#include <functional>

using RenderFunctionType = std::function<void()>;

namespace Engine
{
    namespace Components
    {
        class DebugComponent;
    }

    class Texture2D;
    /**
     * Simple RAII wrapper for the SDL renderer
     */
    class Renderer final : public Singleton<Renderer>
    {
    public:
        void Init(SDL_Window* pWindow);
        // Calls Render() on all the registered components
        void Render() const;

        void Destroy();

        void RenderTexture(Texture2D const& texture, glm::vec2 location) const;
        void RenderTexture(Texture2D const& texture, glm::vec2 location, glm::vec2 dimensions) const;

        [[nodiscard]] SDL_Renderer* GetSDLRenderer() const;

        [[nodiscard]] const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
        void SetBackgroundColor(SDL_Color const& color) { m_clearColor = color; }

        // Adds the function for tracking, so the renderer now calls it
        void RegisterFunction(RenderFunctionType const& renderFunctionToAdd);

        // Removes the function from tracking, so the renderer will not attempt to call it
        void UnregisterFunction(RenderFunctionType const& renderFunctionToRemove);

    private:
        SDL_Renderer* m_pSDLRenderer{};
        SDL_Window* m_pWindow{};
        SDL_Color m_clearColor{  0, 0, 0, 255 };
        std::vector<std::function<void()> const*> m_pRenderFunctions;// Non-owning
        std::vector<std::function<void()> const*> m_pDebugRenderFunctions;// Non-owning

        void InitializeImGui();

    };
}

#endif// SE_RENDERER_H

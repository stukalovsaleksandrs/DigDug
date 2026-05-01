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

    class Sprite;
    /**
     * Simple RAII wrapper for the SDL renderer
     */
    class Renderer final : public Singleton<Renderer>// TODO: Add a service locator for it
    {
    public:
        void Init(SDL_Window*, glm::uvec2 logicalDims);
        // Calls Render() on all the registered components
        void Render() const;

        void Destroy();

        void RenderTexture(Sprite const&, glm::vec2 location) const;
        void RenderTexture(Sprite const&, SDL_FRect const& srcRect, SDL_FRect const& dstRect) const;

        [[nodiscard]] SDL_Renderer* GetSDLRenderer() const{ return m_pSDLRenderer; };

        [[nodiscard]] const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
        void SetBackgroundColor(SDL_Color const& color) { m_clearColor = color; }

        // Adds the function for tracking, so the renderer now calls it
        void RegisterFunction(RenderFunctionType const& renderFunctionToAdd);

        // Removes the function from tracking, so the renderer will not attempt to call it
        void UnregisterFunction(RenderFunctionType const& renderFunctionToRemove);

    private:
        SDL_Renderer* m_pSDLRenderer{};
        SDL_Color m_clearColor{  0, 0, 0, 255 };
        std::vector<std::function<void()> const*> m_pRenderFunctions;// Non-owning
        std::vector<std::function<void()> const*> m_pDebugRenderFunctions;// Non-owning

        void InitializeImGui(SDL_Window* pWindow);

    };
}

#endif// SE_RENDERER_H

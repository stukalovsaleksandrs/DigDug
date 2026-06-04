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
    class Window;

    namespace Components
    {
        class DebugComponent;
    }

    class Sprite;
    /**
     * A simple RAII wrapper around the SDL renderer
     */
    class Renderer final : public Singleton<Renderer>// TODO: Add a service locator for it
    {
    public:
        void Init(Window const&);
        // Calls Render() on all the registered components
        void Render() const;

        void Destroy();

        void RenderTexture(Sprite const&, glm::vec2 location) const noexcept;
        void RenderTexture(Sprite const&, SDL_FRect const& srcRect, SDL_FRect const& dstRect, float degrees, SDL_FlipMode flipMode = SDL_FLIP_NONE) const noexcept;
        void RenderLine(glm::vec2 p1, glm::vec2 p2) const noexcept;
        void RenderFilledCircle(glm::vec2 center, float radius) const noexcept;

        [[nodiscard]] SDL_Renderer* GetSDLRenderer() const noexcept { return m_pSDLRenderer; };

        [[nodiscard]] const SDL_Color& GetBackgroundColor() const noexcept { return m_clearColor; }
        void SetBackgroundColor(SDL_Color const& color) noexcept { m_clearColor = color; }

        // Adds the function for tracking, so the renderer now calls it
        void RegisterFunction(RenderFunctionType const& renderFunctionToAdd) noexcept;

        // Removes the function from tracking, so the renderer will not attempt to call it
        void UnregisterFunction(RenderFunctionType const& renderFunctionToRemove) noexcept;

    private:
        SDL_Renderer* m_pSDLRenderer{};
        Window const* m_pWindow{};// Non-owning
        SDL_Color m_clearColor{  0, 0, 0, 255 };
        std::vector<std::function<void()> const*> m_pRenderFunctions;// Non-owning
        std::vector<std::function<void()> const*> m_pDebugRenderFunctions;// Non-owning

        void InitializeImGui(SDL_Window* pWindow) noexcept;

    };
}

#endif// SE_RENDERER_H

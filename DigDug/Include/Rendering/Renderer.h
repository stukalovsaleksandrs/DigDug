#ifndef RENDERER_H
#define RENDERER_H
#include "../Core/Singleton.h"
#include <SDL3/SDL.h>
#include <glm/vec2.hpp>
#include <vector>

namespace DAE
{
    namespace Components
    {
        class RenderComponent;
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

        // TODO: Accept components by ref & Add unregister to the debug component

        // Adds a RenderComponent for tracking, so the renderer now renders it
        void RegisterComponent(Components::RenderComponent* pRenderComponent);

        void RegisterComponent(Components::DebugComponent* pDebugComponent);

        // Removes the given component from tracking, so the renderer will not attempt to render it
        void UnregisterComponent(Components::RenderComponent* renderComponentToRemove);

    private:
        SDL_Renderer* m_pSDLRenderer{};
        SDL_Window* m_pWindow{};
        SDL_Color m_clearColor{  255, 0, 0, 255 };
        std::vector<Components::RenderComponent*> m_pRenderComponents;// Non-owning
        std::vector<Components::DebugComponent*> m_pDebugComponents;// Non-owning

        void InitializeImGui();
    };
}

#endif

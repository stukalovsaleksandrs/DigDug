#ifndef SE_COMPONENTS_H
#define SE_COMPONENTS_H

// Engine
#include "Engine/Components/ComponentBase.h"
#include "Engine/Rendering/Renderer.h"
// Third-party
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include "glm/ext/scalar_constants.hpp"
#include "glm/vec2.hpp"
// Standard
#include <memory>
#include <string_view>
#include <string>
#include <functional>
#include <optional>

namespace Engine
{
    class GameObject;
    class Texture2D;
    class Font;

    /*******************************************
     * Render component
     *******************************************/

    class RenderComponent final : public Component {
    public:
        explicit RenderComponent(GameObject& owner) noexcept;
        ~RenderComponent() override;
        RenderComponent(RenderComponent const&) = delete;
        RenderComponent(RenderComponent&&) = delete;
        RenderComponent& operator= (RenderComponent const&) = delete;
        RenderComponent& operator= (RenderComponent&&) = delete;

        void Render() const;
        void SetTexture(std::string_view filename);
        void SetTexture(SDL_Texture* pTexture);
        [[nodiscard]] glm::vec2 GetTextureDims() const noexcept;

    private:
        std::shared_ptr<Texture2D> m_pTexture{};
        std::function<void()> m_renderFunction{ [this]{this->Render();} };
        SDL_Rect m_bounds{};

    };

    /*******************************************
    * Debug component
    *******************************************/
    // Interface for components to provide debug rendering
    class DebugComponent : public Component
    {
    public:
        explicit DebugComponent(GameObject& owner, Renderer&) noexcept;
        ~DebugComponent() override;
        DebugComponent(DebugComponent&&) noexcept = delete;
        DebugComponent(DebugComponent const&) noexcept = delete;
        DebugComponent& operator=(DebugComponent const&) noexcept = delete;
        DebugComponent& operator=(DebugComponent&&) noexcept = delete;

        virtual void DebugRender() = 0;

    private:
        std::function<void()> const m_debugRenderFunction{ [this]{this->DebugRender();} };
        Renderer& m_renderer;
    };

    /*******************************************
     * Text component
     *******************************************/
    class TextComponent final : public Component {
    public:
        explicit TextComponent(GameObject &owner,
                               std::string_view text, std::shared_ptr<Font> const& pFont,
                               SDL_Color const&color = {255, 255, 255, 255}) noexcept;
        void SetFont(std::shared_ptr<Font> const& pFont);
        void SetText(std::string_view text);
        void SetColor(SDL_Color const& color);

    private:
        std::string m_text;
        std::shared_ptr<Font> m_pFont;
        SDL_Color m_color{ 255, 255, 255, 255 };
        RenderComponent& m_renderComponent;

        void UpdateTexture() const;
    };

    /*******************************************
     * FPS component
     *******************************************/
    class FPSComponent final : public Component {
    public:
        explicit FPSComponent(GameObject &owner, std::shared_ptr<Font> const& pFont,
                               SDL_Color const& color = {255, 255, 255, 255}) noexcept;
        void Update() noexcept override;
    };

    /*******************************************
     * Orbit component
     *******************************************/
    class OrbitComponent final : public Component
    {
    public:
        /**
         * Rotates the object around @center at the @distance with the speed of radians.
         * Sets the transform value to transform component
         * The distance between the component's parent and parent of the parent is preserved
         * @param owner GameObject the component belongs to
         * @param radiansSec speed, with which the object rotates around the pivot point
         * @note Requires the owner to have a parent
         */
        explicit OrbitComponent(GameObject &owner, float radiansSec) noexcept;
        void Update() noexcept override;

    private:
        float m_radiansSec{ 0.25f * glm::pi<float>() };
    };

}

#endif// SE_COMPONENTS_H

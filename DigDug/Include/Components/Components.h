#ifndef COMPONENT_H
#define COMPONENT_H

#include "ComponentBase.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include "glm/ext/scalar_constants.hpp"
#include <memory>
#include <string_view>
#include <string>

namespace DAE {
    class GameObject;
    class Texture2D;
    class Font;
}

namespace DAE::Components
{
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
        void SetTexture(SDL_Texture* pSDLTexture);

    private:
        std::shared_ptr<Texture2D> m_pTexture{};
    };



    /*******************************************
     * Text component
     *******************************************/
    class TextComponent final : public Component {
    public:
        explicit TextComponent(GameObject &owner, std::string_view text, std::shared_ptr<Font> const& pFont,
                               SDL_Color const&color = {255, 255, 255, 255}) noexcept;
        void SetFont(std::shared_ptr<Font> const& pFont);
        void SetText(std::string_view text);
        void SetColor(SDL_Color const& color);

    private:
        std::string m_text;
        std::shared_ptr<Font> m_pFont;
        SDL_Color m_color{ 255, 255, 255, 255 };
        RenderComponent* m_pRenderComponent;

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

#endif

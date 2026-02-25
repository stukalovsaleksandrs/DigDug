#ifndef COMPONENT_H
#define COMPONENT_H

#include "Transform.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <glm/vec2.hpp>
#include <concepts>
#include <memory>
#include <string_view>
#include <string>

#include "glm/ext/scalar_constants.hpp"

namespace DAE {
    class GameObject;
    class Texture2D;
    class Font;
}

namespace DAE::Components {
    /*******************************************
     * Component base
     *******************************************/
    /** @note Required components get added automatically if absent in the owner */
    class Component {
    public:
        explicit Component(GameObject& owner) noexcept : m_owner(owner){};
        virtual ~Component() noexcept = default;
        Component(Component const&) noexcept = delete;
        Component& operator=(Component const&) noexcept = delete;
        Component(Component&&) noexcept = delete;
        Component& operator=(Component&&) noexcept = delete;

        virtual void Update() noexcept {};

    protected:
        GameObject& m_owner;
    };
    template<typename DerivedComponentType>
    concept DerivedComponent = std::derived_from<DerivedComponentType, Component>;

    class TransformComponent final : public Component {
    public:
        explicit TransformComponent(GameObject& owner) noexcept : Component(owner) {};
        explicit TransformComponent(GameObject& owner, Transform const& transform) noexcept
            : TransformComponent(owner) {
            // Not in the initializer list because member initialization
            // in there is prohibited for delegating constructors
            m_transform = transform;
        }
        [[nodiscard]] Transform const& GetTransform() const noexcept {
            return m_transform;
        }
        void SetLocation(glm::vec2 const location) noexcept {
            m_transform.SetLocation(location);
        }

    private:
        Transform m_transform{};
    };

    /*******************************************
     * Render component
     *******************************************/
    /** @note Requires TransformComponent */
    class RenderComponent final : public Component {
    public:
        explicit RenderComponent(GameObject& owner) noexcept;
        ~RenderComponent() override;
        RenderComponent(RenderComponent const&) = delete;
        RenderComponent(RenderComponent&&) = delete;
        RenderComponent operator= (RenderComponent const&) = delete;
        RenderComponent operator= (RenderComponent&&) = delete;

        void Render() const;
        void SetTexture(std::string_view filename);
        void SetTexture(SDL_Texture* pSDLTexture);

    private:
        std::shared_ptr<Texture2D> m_pTexture{};
        TransformComponent* m_pTransformComponent;
    };

    /*******************************************
     * Text component
     *******************************************/
    /** @note Requires TransformComponent and RenderComponent */
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
    /** @note Requires TextComponent */
    class FPSComponent final : public Component {
    public:
        explicit FPSComponent(GameObject &owner, std::shared_ptr<Font> const& pFont,
                               SDL_Color const& color = {255, 255, 255, 255}) noexcept;
        void Update() noexcept override;
    };

    /*******************************************
     * Orbit component
     *******************************************/
    /** @note Requires TextComponent */
    class OrbitComponent final : public Component
    {
    public:
        /**
         * Rotates the object around @center at the @distance with the speed of radians.
         * Sets the transform value to transform component
         * @param owner GameObject the component belongs to
         * @param origin in pixels, the point around which the character rotates
         * @param distance in pixels from point defined by transform component to the object
         * @param radiansSec speed, with which the object rotates around the pivot point
         * @note Requires TransformComponent
         */
        explicit OrbitComponent(GameObject &owner, glm::vec2 origin, float distance, float radiansSec) noexcept;
        void Update() noexcept override;

    private:
        TransformComponent* m_pTransformComponent;
        glm::vec2 m_origin;
        float m_radiansSec{ 0.25f * glm::pi<float>() },
            m_distance{ 50.f };
    };
}

#endif

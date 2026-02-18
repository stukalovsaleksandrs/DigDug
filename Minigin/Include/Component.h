#ifndef COMPONENT_H
#define COMPONENT_H

#include "Transform.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_render.h>
#include <glm/vec2.hpp>
#include <concepts>
#include <memory>
#include <string_view>


namespace DAE {
    class GameObject;
    class Texture2D;
    class Font;
}

namespace DAE::Components {
    /*******************************************
     * Component base
     *******************************************/
    class Component {
    public:
        virtual ~Component() noexcept = default;
        Component(Component const&) noexcept = delete;
        Component& operator=(Component const&) noexcept = delete;
        Component(Component&&) noexcept = delete;
        Component& operator=(Component&&) noexcept = delete;

        virtual void Update() noexcept {};
    protected:
        GameObject& m_owner;
        explicit Component(GameObject& owner) noexcept : m_owner(owner){};
        friend class DAE::GameObject;// NOTE: Scope specification is mandatory, otherwise
        // the compiler will work with non-existing DAE::Components::GameObject
    };
    template<typename DerivedComponentType>
    concept DerivedComponent = std::derived_from<DerivedComponentType, Component>;

    class TransformComponent final : public Component {
    public:
        [[nodiscard]] Transform const& GetTransform() const noexcept {
            return m_transform;
        }
        void SetLocation(glm::vec2 location) {
            m_transform.SetLocation({location.x, location.y, 0.f});
        }

    protected:
        explicit TransformComponent(GameObject& owner) noexcept : Component(owner) {};
        explicit TransformComponent(GameObject& owner, Transform const& transform) noexcept
            : TransformComponent(owner) {
            // Not in the initializer list because member initialization
            // in there is prohibited for delegating constructors
            m_transform = transform;
        }
        friend class DAE::GameObject;

    private:
        Transform m_transform{};
    };

    /*******************************************
     * Render component
     *******************************************/
    /** @note Requires TransformComponent */
    class RenderComponent final : public Component {
    public:
        void Render() const;
        void SetTexture(std::string_view filename);
        void SetTexture(SDL_Texture* pSDLTexture);

    protected:
        explicit RenderComponent(GameObject& owner) noexcept;
        friend class DAE::GameObject;

    private:
        std::shared_ptr<Texture2D> m_pTexture{};
    };

    /*******************************************
     * Text component
     *******************************************/
    /** @note Requires TransformComponent and RenderComponent */
    class TextComponent final : public Component {
    public:
        void SetFont(std::shared_ptr<Font> const& pFont);
        void SetText(std::string_view text);
        void SetColor(SDL_Color const& color);

    protected:
        explicit TextComponent(GameObject &owner, std::string_view text, std::shared_ptr<Font> const& pFont,
                               SDL_Color const&color = {255, 255, 255, 255}) noexcept;
        friend class DAE::GameObject;

    private:
        std::string m_text;
        std::shared_ptr<Font> m_pFont;
        SDL_Color m_color{ 255, 255, 255, 255 };

        void UpdateTexture() const;
    };

    /*******************************************
     * FPS component
     *******************************************/
    /** @note Requires TextComponent */
    class FPSComponent final : public Component {
    public:
        void Update() noexcept override ;

    protected:
        explicit FPSComponent(GameObject &owner, std::shared_ptr<Font> const& pFont,
                               SDL_Color const& color = {255, 255, 255, 255}) noexcept;
        friend class DAE::GameObject;
    };
}

#endif

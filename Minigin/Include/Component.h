#ifndef COMPONENT_H
#define COMPONENT_H

#include "Transform.h"
#include <glm/vec2.hpp>
#include <concepts>
#include <memory>
#include <string_view>

namespace DAE {
    class GameObject;
    class Texture2D;
}

namespace DAE::Components {
    // TODO: Make non-instantiable in standalone
    class Component {
    public:
        explicit Component(GameObject& owner) noexcept : m_owner(owner){};
        virtual ~Component() noexcept = default;
        Component(Component const&) noexcept = delete;
        Component& operator=(Component const&) noexcept = delete;
        Component(Component&&) noexcept = delete;
        Component& operator=(Component&&) noexcept = delete;
    private:
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
        void SetLocation(glm::vec2 location) {
            m_transform.SetLocation({location.x, location.y, 0.f});
        }

    private:
        Transform m_transform{};
    };

    /**
     * @note Requires TransformComponent
     */
    class RenderComponent final : public Component {
    public:
        explicit RenderComponent(GameObject& owner) noexcept;
        void Render() const;
        void SetTexture(std::string_view filename);

    private:
        std::shared_ptr<Texture2D> m_texture{};
        TransformComponent* m_pOwnerTransformComponent{};
    };

    class TextComponent final : public Component {
    public:
        explicit TextComponent(GameObject& owner) : Component(owner) {};
    };
}

#endif

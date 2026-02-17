#ifndef COMPONENT_H
#define COMPONENT_H

#include <algorithm>

#include "Transform.h"
#include <glm/vec2.hpp>
#include <concepts>

namespace DAE::Components {
    // TODO: Make non-instantiable in standalone
    class Component {
    public:
        Component() = default;
        virtual ~Component() = default;
        Component(Component const&) = default;
        Component& operator=(Component const&) = default;
        Component(Component&&) = default;
        Component& operator=(Component&&) = default;
    };
    template<typename DerivedComponentType>
    concept DerivedComponent = std::derived_from<DerivedComponentType, Component>;

    class TransformComponent final : public Component {
    public:
        TransformComponent() = default;
        explicit TransformComponent(Transform const& transform)
            : m_transform(transform) {}
        [[nodiscard]] Transform const& GetTransform() const {
            return m_transform;
        }
        void SetLocation(glm::vec2 location) {
            m_transform.SetLocation({location.x, location.y, 0.f});
        }

    private:
        Transform m_transform{};
    };

    class RenderComponent final : public Component {

    };

    class TextComponent final : public Component {
    };
}

#endif

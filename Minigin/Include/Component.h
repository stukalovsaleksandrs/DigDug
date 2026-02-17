#ifndef COMPONENT_H
#define COMPONENT_H

#include <concepts>

namespace DAE {
    // TODO: Make instantiable
    class Component {
    public:
        virtual ~Component() = default;
        Component(Component const&) = delete;
        Component(Component&&) = delete;
        Component& operator=(Component const&) = delete;
        Component& operator=(Component&&) = delete;

    };
    template<typename DerivedComponentType>
    concept DerivedComponent = std::derived_from<DerivedComponentType, Component>;

    class RenderComponent final : public Component {
    };

    class TransformComponent final : public Component {
    };

    class TextComponent final : public Component {
    };
}

#endif

#ifndef COMPONENT_H
#define COMPONENT_H

namespace DAE {
    class Component {
    public:
        virtual ~Component() = default;
        Component(Component const&) = delete;
        Component(Component&&) = delete;
        Component& operator=(Component const&) = delete;
        Component& operator=(Component&&) = delete;
    };

    class RenderComponent final : public Component {
    };

    class TransformComponent final : public Component {
    };

    class TextComponent final : public Component {
    };
}

#endif

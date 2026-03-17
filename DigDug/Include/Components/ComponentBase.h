#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H
#include <concepts>
#include <functional>
#include <SDL3/SDL_render.h>

namespace DAE
{
    class GameObject;
}

namespace DAE::Components
{
    /*******************************************
     * Component base
     *******************************************/
    class Component {
    public:
        explicit Component(GameObject& owner) noexcept : m_owner(owner){};
        virtual ~Component() noexcept = default;
        Component(Component const&) noexcept = delete;
        Component& operator=(Component const&) noexcept = delete;
        Component(Component&&) noexcept = delete;
        Component& operator=(Component&&) noexcept = delete;

        virtual void Update() noexcept {};

        [[nodiscard]] GameObject& GetOwner() const noexcept { return m_owner; };

    protected:
        GameObject& m_owner;

    };
    template<typename DerivedComponentType>
    concept DerivedComponent = std::derived_from<DerivedComponentType, Component>;

    /*******************************************
      * Debug component
      *******************************************/
    // Interface for components to provide debug rendering
    class DebugComponent : public Component
    {
    public:
        explicit DebugComponent(GameObject& owner) noexcept;

        virtual void DebugRender() = 0;

    private:
        std::function<void()> const m_debugRender{ [this]{this->DebugRender();} };
    };
}
#endif // COMPONENTBASE_H

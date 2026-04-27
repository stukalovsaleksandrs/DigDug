#ifndef SE_COMPONENT_BASE_H
#define SE_COMPONENT_BASE_H

// Standard
#include <concepts>
#include <functional>

namespace Engine
{
    class GameObject;
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

#endif// SE_COMPONENT_BASE_H

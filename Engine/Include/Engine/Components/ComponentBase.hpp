#ifndef ENGINE_COMPONENT_BASE
#define ENGINE_COMPONENT_BASE

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

}

#endif// ENGINE_COMPONENT_BASE

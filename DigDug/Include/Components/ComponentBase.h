#ifndef COMPONENTBASE_H
#define COMPONENTBASE_H
#include <concepts>

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

    protected:
        GameObject& m_owner;

    };
    template<typename DerivedComponentType>
    concept DerivedComponent = std::derived_from<DerivedComponentType, Component>;

}
#endif // COMPONENTBASE_H

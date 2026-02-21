#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <algorithm>

#include "Component.h"
#include <memory>
#include <optional>

namespace DAE {
    class Texture2D;
    class GameObject final
    {
    public:
        void Update() const;

        /**
         * @def Attempts to add to the parent game object a new component of the type given
         * @return A vector of pointers, where each of them points either to a new component or
         * to an existing one if the owner already had a component with the same type
         */
        template<Components::DerivedComponent ComponentType, typename... Args>
        Components::ComponentWeakPtr<ComponentType> AddComponent(Args&&... args) noexcept {
            // Trying returning the existing component
            for (auto& pComponent : m_pComponents) {
                if (ComponentType* pDerivedComponent{ dynamic_cast<ComponentType*>(pComponent.Get()) }; pDerivedComponent) {
                    return Components::ComponentWeakPtr<ComponentType>(pComponent);
                }
            }
            // Returning a new component since there is no existing one
            // NOTE: Not using std::make_unique since it cannot access protected constructors
            return
                m_pComponents.emplace_back(Components::ComponentUniquePtr<ComponentType>(
                    new ComponentType(std::forward<Args>(args)...)
                )).GetWeakPtr();
        }

        /**
         * @def A helper predicate used to determine whether the input component has the type given
         * @tparam ComponentType Component type to check the input component against
         * @param pComponent The component type of which is checked
         * @return Whether pComponent has the type of ComponentType
         */
        template<Components::DerivedComponent ComponentType>
        static bool IsSameType(std::unique_ptr<Components::Component> const& pComponent) noexcept {
            return dynamic_cast<ComponentType*>(pComponent.get());
        }

        /**
         * @def Finds out if the owner has a component of the type given
         * @return Whether the owner has a component of the type given
         */
        template<Components::DerivedComponent ComponentType>
        [[nodiscard]] bool HasComponent() const noexcept {
            return std::ranges::any_of(m_pComponents, IsSameType<ComponentType>);
        }

        /**
         * @def Attempts to remove the input component from the owner
         * @note Nothing happens if the owner does not have a component of the type given
         */
        template<Components::DerivedComponent ComponentType>
        void RemoveComponent() noexcept {
            std::erase_if(m_pComponents, IsSameType<ComponentType>);
        }

        /**
         * @def Attempts to find the component of the given type at the owner's disposal
         * @return Pointer to the component if such exists; if not, then std::nullopt
         */
        template<Components::DerivedComponent ComponentType>
        [[nodiscard]] std::optional<Components::ComponentWeakPtr<ComponentType>> TryGettingComponent() const noexcept {
            for (auto const& pComponent : m_pComponents) {
                if (ComponentType const* pDerivedComponent{ dynamic_cast<ComponentType*>(pComponent) }; pDerivedComponent) {
                    return Components::ComponentWeakPtr<ComponentType>( pComponent );
                }
            }
            return std::nullopt;
        }

        /**
         * @def Finds the component of the given type at the owner's disposal
         * @note Throws if the owner does not have a component of the type given
         */
        template<Components::DerivedComponent ComponentType>
        Components::ComponentWeakPtr<ComponentType> GetComponent() noexcept {
            if (auto const& pComponent{ TryGettingComponent<ComponentType>() }; pComponent.has_value()) {
                return pComponent.value();
            }
            throw std::runtime_error("Trying GameObject does not the component given");
        }

        private:
            std::vector<Components::ComponentUniquePtr<Components::Component>> m_pComponents{};
    };
}


#endif

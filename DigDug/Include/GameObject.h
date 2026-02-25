#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Component.h"
#include <memory>
#include <optional>
#include <algorithm>

namespace DAE
{
    class Texture2D;
    class Scene;
    class GameObject final
    {
    public:
        void Update() const;

        /**
         * Attempts to add to the parent game object a new component of the type given
         * @return A raw(non-owning) pointer to the newly-added component or
         * to an existing one if the owner already had a component with the same type
         */
        template<Components::DerivedComponent ComponentType, typename... Args>
        ComponentType* AddComponent(Args&&... args) noexcept {
            // Trying returning the existing component
            for (auto& pComponent : m_pComponents) {
                if (ComponentType* pDerivedComponent{ dynamic_cast<ComponentType*>(pComponent.get()) }; pDerivedComponent) {
                    return pDerivedComponent;
                }
            }
            // Returning a new component since there is no existing one
            return dynamic_cast<ComponentType*>(
                m_pComponents.emplace_back(std::unique_ptr<ComponentType>(
                    // NOTE: Not using std::make_unique since it cannot access protected constructors
                    new ComponentType(std::forward<Args>(args)...)
                )).get()
                );
        }

        /**
         * A helper predicate used to determine whether the input component has the type given
         * @tparam ComponentType Component type to check the input component against
         * @param pComponent The component type of which is checked
         * @return Whether pComponent has the type of ComponentType
         */
        template<Components::DerivedComponent ComponentType>
        static bool IsSameType(std::unique_ptr<Components::Component> const& pComponent) noexcept {
            return dynamic_cast<ComponentType*>(pComponent.get());
        }

        /**
         * Finds out if the owner has a component of the type given
         * @return Whether the owner has a component of the type given
         */
        template<Components::DerivedComponent ComponentType>
        [[nodiscard]] bool HasComponent() const noexcept {
            return std::ranges::any_of(m_pComponents, IsSameType<ComponentType>);
        }

        /**
         * Attempts to remove the input component from the owner
         * @note Nothing happens if the owner does not have a component of the type given
         */
        template<Components::DerivedComponent ComponentType>
        void RemoveComponent() noexcept {
            std::erase_if(m_pComponents, IsSameType<ComponentType>);
        }

        /**
         * Attempts to find the component of the given type at the owner's disposal
         * @return Pointer to the component if such exists; if not, then std::nullopt
         */
        template<Components::DerivedComponent ComponentType>
        std::optional<ComponentType*> TryGettingComponent() noexcept {
            for (auto const& pComponent : m_pComponents) {
                if (ComponentType* pDerivedComponent{ dynamic_cast<ComponentType*>(pComponent.get()) }; pDerivedComponent) {
                    return pDerivedComponent;
                }
            }
            return std::nullopt;
        }

        /**
         * Finds the component of the given type at the owner's disposal
         * @note Asserts if the owner does not have a component of the type given
         */
        template<Components::DerivedComponent ComponentType>
        ComponentType* GetComponent() noexcept {
            if (auto const& pComponent{ TryGettingComponent<ComponentType>() }; pComponent.has_value()) {
                return pComponent.value();
            }
            assert(false && "Component not found");
            return nullptr;
        }


    private:
        std::vector<std::unique_ptr<Components::Component>> m_pComponents{};

    };

}

#endif

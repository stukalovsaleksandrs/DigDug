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
        struct DeletableComponent
        {
            // NOTE: Not just adding a deletion flag to the component class,
            // to make sure GameObject is the only one allowed to delete
            // its components
            std::unique_ptr<Components::Component> pComponent;
            bool markedForDeletion{};
        };

    public:
        void Update();

        /**
         * Attempts to add to the parent game object a new component of the type given
         * @return A raw(non-owning) pointer to the newly-added component or
         * to an existing one if the owner already had a component with the same type
         */
        template<Components::DerivedComponent ComponentType, typename... Args>
        ComponentType* AddComponent(Args&&... args) noexcept {
            // Trying returning the existing component
            for (auto& [pComponent, markedForDeletion] : m_components) {
                if (ComponentType* pDerivedComponent{ dynamic_cast<ComponentType*>(pComponent.get()) }; pDerivedComponent) {
                    return pDerivedComponent;
                }
            }
            // Returning a new component since there is no existing one
            m_components.emplace_back(
                DeletableComponent(
                    std::unique_ptr<ComponentType>(
                        // NOTE: Not using std::make_unique since it cannot access protected constructors
                        new ComponentType(std::forward<Args>(args)...)
                    ),
                    false
                )
            );
            return dynamic_cast<ComponentType*>(m_components.back().pComponent.get());
        }

        /**
         * A helper predicate used to determine whether the input component has the type given
         * @tparam ComponentType Component type to check the input component against
         * @param component The component type of which is checked
         * @return Whether pComponent has the type of ComponentType
         */
        template<Components::DerivedComponent ComponentType>
        static bool IsSameType(DeletableComponent const& component) noexcept {
            return dynamic_cast<ComponentType*>(component.pComponent.get());
        }

        /**
         * Finds out if the owner has a component of the type given
         * @return Whether the owner has a component of the type given
         */
        template<Components::DerivedComponent ComponentType>
        [[nodiscard]] bool HasComponent() const noexcept {
            return std::ranges::any_of(m_components, IsSameType<ComponentType>);
        }

        /**
         * Attempts to remove the input component from the owner
         * @note Nothing happens if the owner does not have a component of the type given
         */
        template<Components::DerivedComponent ComponentType>
        void RemoveComponent() noexcept {
            m_anyComponentsToDelete = true;
            if (auto componentIterator{ std::ranges::find_if(m_components, IsSameType<ComponentType>)};
                componentIterator != m_components.end())
            {
                componentIterator->markedForDeletion = true;
            }
        }

        /**
         * Attempts to find the component of the given type at the owner's disposal
         * @return Pointer to the component if such exists; if not, then std::nullopt
         */
        template<Components::DerivedComponent ComponentType>
        std::optional<ComponentType*> TryGettingComponent() noexcept {
            for (const auto& [pComponent, markedForDeletion] : m_components) {
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
        std::vector<DeletableComponent> m_components{};
        bool m_anyComponentsToDelete{};

        void DeleteMarkedComponents() noexcept;
    };

}

#endif

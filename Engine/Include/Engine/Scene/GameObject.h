#ifndef SE_GAME_OBJECT_H
#define SE_GAME_OBJECT_H

// Engine
#include "Engine/Scene/HierarchyElement.h"
#include "Engine/Components/Components.h"
// Third-party
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
// Standard
#include <memory>
#include <optional>
#include <algorithm>
#include <vector>

namespace Engine
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
            std::unique_ptr<Component> pComponent;
            bool markedForDeletion{};
        };

        bool m_markedForDeletion{};

    public:
        HierarchyElement hierarchyElement;
        explicit GameObject(Scene& scene, glm::vec2 localPosition = {}) noexcept;

        /*******************************************
         * Lifetime
         *******************************************/

        void Update();

        void MarkForDeletion() noexcept;
        [[nodiscard]] bool IsMarkedForDeletion() const noexcept;

        /*******************************************
         * Components
         *******************************************/


        /**
         * Adds to the owner game object a new component of the type given or returns an existing one
         * @return A reference to the newly-added component or to the existing one
         */
        template<DerivedComponent ComponentType, typename... Args>
        ComponentType& AddComponent(Args&&... args) noexcept {
            // TODO: Add a setting controlling whether a component can be added multiple times to the same object or not

            // Trying returning the existing component
            for (auto& [pComponent, markedForDeletion] : m_components) {
                if (ComponentType* pDerivedComponent{ dynamic_cast<ComponentType*>(pComponent.get()) }; pDerivedComponent) {
                    return *pDerivedComponent;
                }
            }

            // Returning a new component since there is no existing one
            m_components.emplace_back(
                DeletableComponent(
                    std::unique_ptr<ComponentType>(
                        // NOTE: Not using std::make_unique since it cannot access protected constructors
                        new ComponentType(*this, std::forward<Args>(args)...)
                    ),
                    false
                )
            );
            return *dynamic_cast<ComponentType*>(m_components.back().pComponent.get());
        }

        /**
         * A helper predicate used to determine whether the input component has the type given
         * @tparam ComponentType Component type to check the input component against
         * @param component The component type of which is checked
         * @return Whether pComponent has the type of ComponentType
         */
        template<DerivedComponent ComponentType>
        static bool IsSameType(DeletableComponent const& component) noexcept {
            return dynamic_cast<ComponentType*>(component.pComponent.get());
        }

        /**
         * Finds out if the owner has a component of the type given
         * @return Whether the owner has a component of the type given
         */
        template<DerivedComponent ComponentType>
        [[nodiscard]] bool HasComponent() const noexcept {
            return std::ranges::any_of(m_components, IsSameType<ComponentType>);
        }

        /**
         * Attempts to remove the component of the input type from the owner
         * @note Nothing happens if the owner does not have a component of the type given
         */
        template<DerivedComponent ComponentType>
        void RemoveComponent() noexcept {
            m_componentDeletionFlagsDirty = true;
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
        template<DerivedComponent ComponentType>
        std::optional<ComponentType*> TryGettingComponent() noexcept {
            for (auto const& [pComponent, markedForDeletion] : m_components) {
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
        template<DerivedComponent ComponentType>
        ComponentType* GetComponent() noexcept {
            // TODO: If there can be multiple components of the same type, return a subrange. Note that
            // there will be a need to sort the components once a component is added then
            if (auto const& pComponent{ TryGettingComponent<ComponentType>() }; pComponent.has_value()) {
                return pComponent.value();
            }
            assert(false && "Component not found");
            return nullptr;
        }


        /*******************************************
         * Transform
         *******************************************/

        void SetLocalPosition(glm::vec2 position) noexcept;
        [[nodiscard]] glm::vec2 GetLocalPosition() const noexcept;

        // NOTE: Non-const, because it can update the position
        [[nodiscard]] glm::vec2 GetWorldPosition() noexcept;

        // Marks as dirty the position of the current game object and all its children
        void SetPositionDirty() noexcept;

        void UpdateWorldPosition() noexcept;

        [[nodiscard]] bool IsDirectChildOfScene() const noexcept;

    private:
        std::vector<DeletableComponent> m_components{};
        bool m_componentDeletionFlagsDirty{};

        glm::vec2 m_localPosition{}, m_worldPosition{};
        bool m_positionIsDirty{};

        Scene& m_scene;

        // Removes all the components with the deletion flag set
        void DeleteMarkedComponents() noexcept;

    };

}

#endif// SE_GAME_OBJECT_H

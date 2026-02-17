#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include <algorithm>

#include "Component.h"
#include <memory>
#include <optional>

namespace DAE
{
    class Texture2D;
    // TODO: Make final
    class GameObject
    {
    public:
        GameObject() noexcept = default;
        virtual ~GameObject() noexcept ;
        GameObject(GameObject const& other) noexcept  = delete;
        GameObject(GameObject&& other) noexcept  = delete;
        GameObject& operator=(GameObject const& other) noexcept = delete;
        GameObject& operator=(GameObject&& other) noexcept = delete;

        virtual void Update();

        /**
         * @def Attempts to add to the parent game object a new component of the type given
         * @return A vector of pointers, where each of them points either to a new component or
         * to an existing one if the owner already had a component with the same type
         */
        template<Components::DerivedComponent ComponentType>
        ComponentType* AddComponent() noexcept {
            for (auto& pComponent : m_pComponents) {
                if (ComponentType* pDerivedComponent{ dynamic_cast<ComponentType*>(pComponent.get()) }; pDerivedComponent) {
                    return pDerivedComponent;
                }
            }
            m_pComponents.emplace_back(std::make_unique<ComponentType>(*this));
            return GetComponent<ComponentType>().value();
        }
        /**
        * @def Attempts to add to the parent game object new components of the types given
        * @return A vector of pointers, where each of them points either to a new component or
        * to an existing one if the owner already had a component with the same type
        */
        template<Components::DerivedComponent... ComponentTypes>
        std::vector<Components::Component*>&& AddComponents() noexcept {
            std::vector<Components::Component*> result(sizeof...(ComponentTypes));
            (result.emplace_back(AddComponent<ComponentTypes>()), ...);
            return std::move(result);
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
         */
        template<Components::DerivedComponent ComponentType>
        std::optional<ComponentType*> GetComponent() noexcept {
            for (auto const& pComponent : m_pComponents) {
                if (ComponentType* pDerivedComponent{ dynamic_cast<ComponentType*>(pComponent.get()) }; pDerivedComponent) {
                    return pDerivedComponent;
                }
            }
            return std::nullopt;
        }

    private:
        std::vector<std::unique_ptr<Components::Component>> m_pComponents{};

    };

}

#endif

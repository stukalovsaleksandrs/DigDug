#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Component.h"
#include <string_view>
#include <memory>
#include <optional>

namespace DAE
{
    class Texture2D;
    // TODO: Make final
    class GameObject
    {
    public:
        GameObject() = default;
        virtual ~GameObject();
        GameObject(GameObject const& other) = delete;
        GameObject(GameObject&& other) = delete;
        GameObject& operator=(GameObject const& other) = delete;
        GameObject& operator=(GameObject&& other) = delete;

        virtual void Update();
        virtual void Render();// Non-const since has to access components
        void SetTexture(std::string_view filename);

        /**
         * @def Attempts to add to the parent game object
         * a new component of the type given
         * @return Whether a component does not exist already
         */
        template<Components::DerivedComponent ComponentType>
        std::optional<ComponentType*> AddComponent() {
            if (HasComponent<ComponentType>()) return std::nullopt;
            auto& newComponent{
                m_pComponents.emplace_back(std::make_unique<ComponentType>())
            };
            return dynamic_cast<ComponentType*>(newComponent.get());
        }
        template<Components::DerivedComponent ComponentType>
        std::optional<ComponentType*> AddComponent(ComponentType const& component) {
            return AddComponent<ComponentType>() = component;
        }

        /**
         * @def A helper predicate used to determine whether the input component has the type given
         * @tparam ComponentType Component type to check the input component against
         * @param pComponent The component type of which is checked
         * @return Whether pComponent has the type of ComponentType
         */
        template<Components::DerivedComponent ComponentType>
        static bool IsSameType(std::unique_ptr<Components::Component> const& pComponent) {
            return dynamic_cast<ComponentType*>(pComponent.get());
        }

        /**
         * @def Finds out if the owner has a component of the type given
         * @return Whether the owner has a component of the type given
         */
        template<Components::DerivedComponent ComponentType>
        [[nodiscard]] bool HasComponent() const {
            return std::ranges::any_of(m_pComponents, IsSameType<ComponentType>);
        }

        /**
         * @def Attempts to remove the input component from the owner
         * @note Nothing happens if the owner does not have a component of the type given
         */
        template<Components::DerivedComponent ComponentType>
        void RemoveComponent() {
            std::erase_if(m_pComponents, IsSameType<ComponentType>);
        }

        /**
         * @def Attempts to find the component of the given type at the owner's disposal
         */
        template<Components::DerivedComponent ComponentType>
        std::optional<ComponentType*> GetComponent() {
            for (auto const& pComponent : m_pComponents) {
                if (ComponentType* pDerivedComponent{ dynamic_cast<ComponentType*>(pComponent.get()) }; pDerivedComponent) {
                    return pDerivedComponent;
                }
            }
            return std::nullopt;
        }

    private:
        std::shared_ptr<Texture2D> m_texture{};
        std::vector<std::unique_ptr<Components::Component>> m_pComponents{};

    };


}

#endif

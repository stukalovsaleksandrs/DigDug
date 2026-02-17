#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Transform.h"
#include <glm/vec2.hpp>
#include <string_view>
#include <memory>
#include "Component.h"

namespace DAE
{
    class Texture2D;
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
        virtual void Render() const;
        void SetTexture(std::string_view filename);
        virtual void SetLocation(glm::vec2);// See C++ Core Guidelines F.16 and C.1

        /**
         * @def Attempts to add to the parent game object
         * a new component of the type given
         * @return Whether a component does not exist already
         */
        template<DerivedComponent ComponentType>
        bool AddComponent(ComponentType const&);

        /**
         * @return Whether the owner has a component of the type given
         */
        template<DerivedComponent ComponentType>
        [[nodiscard]] bool HasComponent() const;

        /**
         * @def Attempts to remove from the owner the component given
         * @note Nothing happens if the owner does not have a component of the type given
         */
        template<DerivedComponent ComponentType>
        void RemoveComponent(ComponentType const&);

    private:
        Transform m_transform{};
        std::shared_ptr<Texture2D> m_texture{};
        std::vector<std::unique_ptr<Component>> m_pComponents{};

    };


}

#endif

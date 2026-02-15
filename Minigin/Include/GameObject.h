#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Transform.h"
#include <glm/vec2.hpp>
#include <string_view>
#include <memory>

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

    private:
        Transform m_transform{};
        std::shared_ptr<Texture2D> m_texture{};

    };
}

#endif

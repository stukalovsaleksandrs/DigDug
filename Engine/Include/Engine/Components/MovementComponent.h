#ifndef SE_INPUT_COMPONENT_H
#define SE_INPUT_COMPONENT_H
// Engine
#include "Engine/Components/ComponentBase.h"
// Third-parth
#include "glm/vec2.hpp"

namespace Engine
{
    class MovementComponent final : public Component
    {
    public:
        explicit MovementComponent(GameObject& owner, float pxPerSec) noexcept;
        void Update() noexcept override;
        void AddDirection(glm::vec2 direction) noexcept;

    private:
        float m_pxPerSec{};
        glm::vec2 m_direction{};

    };

}

#endif// SE_INPUT_COMPONENT_H

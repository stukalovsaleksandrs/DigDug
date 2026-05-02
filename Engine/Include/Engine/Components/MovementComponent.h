#ifndef SE_INPUT_COMPONENT_H
#define SE_INPUT_COMPONENT_H

// Engine
#include "Engine/Components/ComponentBase.h"
#include "Engine/Core/Observer.h"
// Third-parth
#include "glm/vec2.hpp"

namespace Engine
{
    // TODO: Remove ability for diagonal movement
    class MovementComponent final : public Component, public Subject
    {
    public:
        explicit MovementComponent(GameObject& owner, float pxPerSec) noexcept;
        void Update() noexcept override;
        void AddDirection(glm::vec2 direction) noexcept;
        [[nodiscard]] glm::vec2 GetDirection() const noexcept{ return m_direction; };

    private:
        float m_pxPerSec{};
        glm::vec2 m_direction{}, m_prevDirection{};

    };

}

#endif// SE_INPUT_COMPONENT_H

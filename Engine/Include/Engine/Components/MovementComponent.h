#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H
#include "ComponentBase.h"
#include "glm/vec2.hpp"

namespace DAE::Components
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

#endif // INPUTCOMPONENT_H

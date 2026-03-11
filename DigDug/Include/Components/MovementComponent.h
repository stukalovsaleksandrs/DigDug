#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H
#include "ComponentBase.h"
#include "glm/vec2.hpp"

namespace DAE::Components
{
    class MovementComponent final : public Component
    {
    public:
        // Normalized before taking into account. Reset after every update
        glm::vec2 direction{};

        explicit MovementComponent(GameObject& owner) noexcept;
        void Update() noexcept override;
    };

}

#endif // INPUTCOMPONENT_H

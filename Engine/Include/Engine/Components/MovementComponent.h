#ifndef SE_INPUT_COMPONENT_H
#define SE_INPUT_COMPONENT_H

// Engine
#include "Engine/Components/ComponentBase.h"
#include "Engine/Core/Observer.h"
#include "Engine/Core/Window.h"
// Third-parth
#include "glm/vec2.hpp"

namespace Engine
{
    // TODO: Remove ability for diagonal movement
    class MovementComponent final : public Component, public Subject
    {
    public:
        struct Dependencies final
        {
            Window::Data const& windowData;
            glm::uvec2 characterDims;
        };
        explicit MovementComponent(GameObject& owner, Dependencies const&, float pxPerSec) noexcept;
        void Update() noexcept override;
        void AddDirection(glm::vec2 direction) noexcept;
        [[nodiscard]] glm::vec2 GetDirection() const noexcept{ return m_direction; };
        [[nodiscard]] bool IsMoving() const noexcept;

    private:
        Dependencies m_dependencies;
        float m_pxPerSec{};
        glm::vec2 m_direction{}, m_prevDirection{};
        // It will not get more complex, I do not need a state machine for this, pinky promise
        bool m_moving{};

        bool IsWithinScreen(glm::vec2 topLeft) const;
    };

}

#endif// SE_INPUT_COMPONENT_H

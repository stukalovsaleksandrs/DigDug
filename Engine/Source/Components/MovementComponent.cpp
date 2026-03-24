#include "Components/MovementComponent.h"
#include "Utils/Timer.h"
#include "Scene/GameObject.h"
#include <SDL3/SDL_keyboard.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

DAE::Components::MovementComponent::MovementComponent(GameObject& owner, float pxPerSec) noexcept
    : Component(owner)
    , m_pxPerSec{ pxPerSec }
{}

void DAE::Components::MovementComponent::Update() noexcept
{
    if (glm::length2(m_direction) < glm::epsilon<float>()) return;

    m_owner.SetLocalPosition(
        m_owner.GetLocalPosition() + glm::normalize(m_direction) * m_pxPerSec * Timer::GetInstance().GetDeltaSec()
    );

    m_direction = glm::vec2{};
}

void DAE::Components::MovementComponent::AddDirection(glm::vec2 const direction) noexcept
{
    m_direction += direction;
}

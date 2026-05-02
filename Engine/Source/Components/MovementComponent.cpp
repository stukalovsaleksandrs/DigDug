// Engine
#include "Components/MovementComponent.h"
#include "Utils/Timer.h"
#include "Utils/Utils.h"
#include "Utils/Constants.h"
#include "Scene/GameObject.h"
// Third-party
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

Engine::MovementComponent::MovementComponent(GameObject& owner, float const pxPerSec) noexcept
    : Component{owner}
    , m_pxPerSec{ pxPerSec }
{}

void Engine::MovementComponent::Update() noexcept
{
    if (glm::length2(m_direction) < glm::epsilon<float>()) return;

    m_owner.SetLocalPosition(
        m_owner.GetLocalPosition() + glm::normalize(m_direction) * m_pxPerSec * Timer::GetInstance().GetDeltaSec()
    );

    // Firing event if direction changed
    if (!Utils::NearlyEqual(m_prevDirection, m_direction) )
    {
        NotifyObservers(
            Event{
                std::to_underlying(EventType::OnDirectionChanged)
            }
        );
    }

    m_prevDirection = std::exchange(m_direction, {});
}

void Engine::MovementComponent::AddDirection(glm::vec2 const direction) noexcept
{
    m_direction += direction;
}

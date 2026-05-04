// Engine
#include "Components/MovementComponent.h"
#include "Utils/Timer.h"
#include "Utils/Utils.h"
#include "Utils/Constants.h"
#include "Scene/GameObject.h"
// Third-party
#define GLM_ENABLE_EXPERIMENTAL
#include "Core/Window.h"
#include "glm/gtx/norm.hpp"

Engine::MovementComponent::MovementComponent(GameObject& owner, Dependencies const& dependencies, uint32_t const verticalPadding,
    float const pxPerSec) noexcept
    : Component{owner}
    , m_dependencies{ dependencies }
    , m_pxPerSec{ pxPerSec }
    , m_verticalPadding{ verticalPadding }
{}

bool Engine::MovementComponent::IsWithinScreen(glm::vec2 const topLeft) const
{
    // std::println("X: {}/{} Y: {}/{}", topLeft.x + dims.x, Game::windowData.logicalDims.x, topLeft.y + dims.y, Game::windowData.logicalDims.y);
    return topLeft.x > 0.f && topLeft.x + m_dependencies.characterDims.x < static_cast<int>(m_dependencies.windowData.logicalDims.x) &&
        topLeft.y > m_verticalPadding && topLeft.y + m_dependencies.characterDims.y < static_cast<int>(m_dependencies.windowData.logicalDims.y) - m_verticalPadding;
}

void Engine::MovementComponent::Update() noexcept
{
    // Updating the moving variable
    if (glm::length2(m_direction) < glm::epsilon<float>())
    {
        m_moving = false;
        return;
    }
    m_moving = true;

    // Limiting player only to the screen borders
    if (auto const deltaPosition{ glm::normalize(m_direction) * m_pxPerSec * Timer::GetInstance().GetDeltaSec() };
        IsWithinScreen(m_owner.GetWorldPosition() + deltaPosition))
    {
        // Updating location
        m_owner.SetLocalPosition(
            m_owner.GetLocalPosition() + deltaPosition
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
    }

    // Saving the direction
    m_prevDirection = std::exchange(m_direction, {});
}
void Engine::MovementComponent::AddDirection(glm::vec2 const direction) noexcept
{
    m_direction += direction;
}

bool Engine::MovementComponent::IsMoving() const noexcept
{
    return m_moving;
}

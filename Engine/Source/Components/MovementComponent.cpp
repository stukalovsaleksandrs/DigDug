// Engine
#include "Core/Window.hpp"
#include "Components/MovementComponent.hpp"

#include <utility>
#include "Utils/Timer.hpp"
#include "Utils/Utils.hpp"
#include "Utils/Constants.hpp"
#include "Scene/GameObject.hpp"
// Third-party
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

Engine::MovementComponent::MovementComponent(GameObject& owner, Dependencies const& dependencies, uint32_t const verticalPadding,
    float const pxPerSec, CanMovePred canMovePred) noexcept
    : Component{owner}
    , m_dependencies{ dependencies }
    , m_pxPerSec{ pxPerSec }
    , m_verticalPadding{ verticalPadding }
    , m_canMovePred{ std::move(canMovePred) }
{}

bool Engine::MovementComponent::IsWithinScreen(glm::vec2 const topLeft) const
{
    // std::println("X: {}/{} Y: {}/{}", topLeft.x + dims.x, Game::windowData.logicalDims.x, topLeft.y + dims.y, Game::windowData.logicalDims.y);
    static float constexpr epsilon{ 0.5f };
    return topLeft.x >= -epsilon && topLeft.x + static_cast<float>(m_dependencies.tileSideLength) <= static_cast<float>(m_dependencies.windowData.logicalDims.x) + epsilon &&
        topLeft.y >= (m_verticalPadding-epsilon) && topLeft.y + m_dependencies.tileSideLength <= static_cast<int>(m_dependencies.windowData.logicalDims.y) - (m_verticalPadding - epsilon);
}

void Engine::MovementComponent::Update() noexcept
{
    if (m_disabled) return;
    // Updating the moving variable
    if (glm::length2(m_direction) < glm::epsilon<float>())
    {
        m_moving = false;
        return;
    }
    m_moving = true;

    // Limiting player only to the screen borders
    float const deltaSec{ std::min(Timer::GetInstance().GetDeltaSec(), .01f) };
    auto const deltaLocation{ glm::normalize(m_direction) * m_pxPerSec * deltaSec };
    auto const newLocation{ m_owner.GetWorldTopLeft() + deltaLocation };
    if (IsWithinScreen(newLocation))
    {
        if (m_canMovePred(m_owner.GetWorldTopLeft() + deltaLocation))
        {
            // Updating location
            m_owner.SetLocalTopLeft(
                m_owner.GetLocalTopLeft() + deltaLocation
            );

            // Firing event if direction changed
            if (!Utils::NearlyEqual(m_prevDirection, m_direction) && !Utils::NearlyEqual(m_prevDirection, {}))
            {
                // Dispatching that direction changed in general
                NotifyObservers(
                    Event{
                        std::to_underlying(EventType::OnDirectionChanged)
                    }
                );

                // Dispatching whether the movement axis changed
                if (Utils::NearlyZero(glm::dot(m_prevDirection, m_direction)))
                {
                    NotifyObservers(
                        Event{
                            std::to_underlying(EventType::OnMovementAxisChanged)
                        }
                    );
                }
            }
        }
    }

    // Saving the direction
    m_prevDirection = std::exchange(m_direction, {});
}

void Engine::MovementComponent::AddDirection(glm::vec2 const direction) noexcept
{
    if (m_canMoveDiagonally) m_direction += direction;
    else m_direction = direction;
}

bool Engine::MovementComponent::IsMoving() const noexcept
{
    return m_moving;
}

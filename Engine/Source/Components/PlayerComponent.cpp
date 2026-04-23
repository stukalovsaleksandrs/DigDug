#include "Components/PlayerComponent.h"
#include "Components/MovementComponent.h"
#include <print>

DAE::Components::PlayerComponent::PlayerComponent(GameObject& owner) noexcept
    : Component(owner)
    , m_movementComponent(*owner.GetComponent<MovementComponent>())
{}

DAE::Components::PlayerComponent::~PlayerComponent() noexcept
{}

void DAE::Components::PlayerComponent::OnNotify(Event const event, Subject const&) noexcept
{
    // TODO: Find a proper way to bind functions to the events directly
    switch (event.id)
    {
    case MakeSDBMHash("OnDied"):
        {
            m_owner.MarkForDeletion();
            std::println("Player died");
            break;
        }
    default: ;
    }
}

void DAE::Components::PlayerComponent::AddPoints(uint32_t const points) noexcept
{
    m_points += points;
    subject.NotifyObservers(m_onPointsIncreased);
    if (m_points == 5) subject.NotifyObservers(m_onCollected5Points);
}

#include "Components/PlayerComponent.h"
#include "Components/MovementComponent.h"
#include <print>

DAE::Components::PlayerComponent::PlayerComponent(GameObject& owner) noexcept
    : Component(owner)
    , m_movementComponent(*owner.GetComponent<MovementComponent>())
{
    BindInput();
}

DAE::Components::PlayerComponent::~PlayerComponent() noexcept
{
    UnbindInput();
}

void DAE::Components::PlayerComponent::BindInput()
{
    Input::InputManager& inputManager{ Input::InputManager::GetInstance() };
    // Movement
    // Keyboard
    inputManager.Bind(m_upAction, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, -1.f }));
    inputManager.Bind(m_leftAction, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ -1.f, 0.f }));
    inputManager.Bind(m_downAction, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, 1.f }));
    inputManager.Bind(m_rightAction, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 1.f, 0.f }));
    inputManager.Bind(m_pointAction, std::make_unique<Input::PointCommand>(*this));
    /// TODO: Gamepad
}

void DAE::Components::PlayerComponent::UnbindInput() const
{
    Input::InputManager& inputManager{ Input::InputManager::GetInstance() };
    inputManager.Unbind(m_upAction);
    inputManager.Unbind(m_leftAction);
    inputManager.Unbind(m_downAction);
    inputManager.Unbind(m_rightAction);
    inputManager.Unbind(m_pointAction);
}

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

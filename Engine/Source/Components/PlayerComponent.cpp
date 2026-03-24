#include "Components/PlayerComponent.h"
#include "Components/MovementComponent.h"
#include <print>

DAE::Components::PlayerComponent::PlayerComponent(GameObject& owner) noexcept
    : Component(owner)
    , m_movementComponent(*owner.GetComponent<MovementComponent>())
{
    BindInput();

}

void DAE::Components::PlayerComponent::BindInput()
{
    Input::InputManager& inputManager{ Input::InputManager::GetInstance() };
    // Movement
    // Keyboard
    inputManager.Bind({SDL_SCANCODE_W, DAE::Input::InputType::pressed}, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, -1.f }));
    inputManager.Bind({SDL_SCANCODE_A, DAE::Input::InputType::pressed}, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ -1.f, 0.f }));
    inputManager.Bind({SDL_SCANCODE_S, DAE::Input::InputType::pressed}, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, 1.f }));
    inputManager.Bind({SDL_SCANCODE_D, DAE::Input::InputType::pressed}, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 1.f, 0.f }));
    inputManager.Bind({SDL_SCANCODE_P, DAE::Input::InputType::released}, std::make_unique<Input::PointCommand>(*this));
    /// TODO: Gamepad
}

void DAE::Components::PlayerComponent::UnbindInput()
{
    // Input::InputManager& inputManager{ Input::InputManager::GetInstance() };
    // inputManager.Unbind(m_upAction);
    // inputManager.Unbind(m_leftAction);
    // inputManager.Unbind(m_downAction);
    // inputManager.Unbind(m_rightAction);
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

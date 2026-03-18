#include "Components/PlayerComponent.h"

#include <print>

#include "Components/MovementComponent.h"

DAE::Components::PlayerComponent::PlayerComponent(GameObject& owner)
    : Component(owner)
    , m_movementComponent(*owner.GetComponent<MovementComponent>())
{
    BindInput();

}

void DAE::Components::PlayerComponent::BindInput() const
{
    Input::InputManager& inputManager{ Input::InputManager::GetInstance() };
    // Movement
    // Keyboard
    inputManager.Bind(m_upAction, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, -1.f }));
    inputManager.Bind(m_leftAction, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ -1.f, 0.f }));
    inputManager.Bind(m_downAction, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, 1.f }));
    inputManager.Bind(m_rightAction, std::make_unique<Input::MoveCommand>(m_movementComponent, glm::vec2{ 1.f, 0.f }));
    /// TODO: Gamepad
}

void DAE::Components::PlayerComponent::UnbindInput() const
{
    Input::InputManager& inputManager{ Input::InputManager::GetInstance() };
    inputManager.Unbind(m_upAction);
    inputManager.Unbind(m_leftAction);
    inputManager.Unbind(m_downAction);
    inputManager.Unbind(m_rightAction);
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

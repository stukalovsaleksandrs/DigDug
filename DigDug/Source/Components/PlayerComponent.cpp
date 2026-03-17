#include "Components/PlayerComponent.h"

#include <print>

#include "Components/MovementComponent.h"
#include "Input/InputManager.h"

DAE::Components::PlayerComponent::PlayerComponent(MovementComponent& movementComponent)
    : Component(movementComponent.GetOwner())
    , m_movementComponent(movementComponent)
{
    BindInput();

}

void DAE::Components::PlayerComponent::BindInput() const
{
    // Input bindings
    Input::InputManager& inputManager{ Input::InputManager::GetInstance() };
    // Movement
    // Keyboard
    inputManager.Bind({SDL_SCANCODE_W, DAE::Input::InputType::pressed}, std::make_unique<DAE::Input::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, -1.f }));
    inputManager.Bind({SDL_SCANCODE_A, DAE::Input::InputType::pressed}, std::make_unique<DAE::Input::MoveCommand>(m_movementComponent, glm::vec2{ -1.f, 0.f }));
    inputManager.Bind({SDL_SCANCODE_S, DAE::Input::InputType::pressed}, std::make_unique<DAE::Input::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, 1.f }));
    inputManager.Bind({SDL_SCANCODE_D, DAE::Input::InputType::pressed}, std::make_unique<DAE::Input::MoveCommand>(m_movementComponent, glm::vec2{ 1.f, 0.f }));
    /// TODO: Gamepad
}

void DAE::Components::PlayerComponent::OnNotify(Event const event, Subject const&) noexcept
{
    // TODO: Bind functions to the events directly. Do not use OnNotify.
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

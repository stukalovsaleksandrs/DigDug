#include "Components/LivesComponent.h"
#include "Scene/GameObject.h"
#include "Input/InputManager.h"
#include <print>

DAE::Components::LivesComponent::LivesComponent(GameObject& owner, uint32_t const lives) noexcept
    : Component(owner)
    , m_lives(lives)
{
    Input::InputManager& inputManager{ Input::InputManager::GetInstance() };
    inputManager.Bind({SDL_SCANCODE_K, DAE::Input::InputType::released}, std::make_unique<DAE::Input::TakeDamageCommand>(*this));
}

void DAE::Components::LivesComponent::TakeDamage() noexcept
{
    if (!m_lives) return;// Corpse does not care about damage
    --m_lives;
    subject.NotifyObservers(m_onDamageTaken);
    if (!m_lives)// Just died
    {
        subject.NotifyObservers(m_onDiedEvent);
        return;
    }
    std::println("Took damage, lives: {}", m_lives);
}

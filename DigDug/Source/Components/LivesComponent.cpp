#include "Components/LivesComponent.h"
#include "Scene/GameObject.h"
#include "Input/InputManager.h"
#include <print>

DAE::Components::LivesComponent::LivesComponent(GameObject& owner, uint32_t const lives) noexcept
    : Component(owner)
    , m_lives(lives)
    , m_onDiedEvent(MakeSDBMHash("OnDied"))
{
    Input::InputManager& inputManager{ Input::InputManager::GetInstance() };
    inputManager.Bind({SDL_SCANCODE_K, DAE::Input::InputType::released}, std::make_unique<DAE::Input::TakeDamageCommand>(*this));
}

DAE::Components::LivesComponent::~LivesComponent() noexcept
{
    // Notifying all the observers that the subject got deleted
    // NotifyObservers(m_subjectDeletedEvent);// The observer is actually deleted by this point:/
}

void DAE::Components::LivesComponent::TakeDamage() noexcept
{
    if (!m_lives) return;// Corpse does not care about damage
    --m_lives;
    if (!m_lives)// Just died
    {
        NotifyObservers(m_onDiedEvent);
        return;
    }
    std::println("Took damage, lives: {}", m_lives);
}

// Game
#include "Components/LivesComponent.h"
// Engine
#include "Engine/Scene/GameObject.h"
#include "Engine/InputManager.h"
// Standard
#include <print>

#include "Commands.h"


Game::LivesComponent::LivesComponent(Engine::GameObject& owner, uint32_t const lives) noexcept
    : Component(owner)
    , m_lives(lives)
{
    Engine::InputManager& inputManager{ Engine::InputManager::GetInstance() };
    inputManager.Bind({SDL_SCANCODE_K, Engine::InputType::released}, std::make_unique<TakeDamageCommand>(*this));
}

void Game::LivesComponent::TakeDamage() noexcept
{
    if (!m_lives) return;// Corpses do not care about damage
    --m_lives;
    subject.NotifyObservers(m_onDamageTaken);
    if (!m_lives)// Just died
    {
        subject.NotifyObservers(m_onDiedEvent);
        return;
    }
    std::println("Took damage, lives: {}", m_lives);
}

uint32_t Game::LivesComponent::GetLives() const noexcept
{
    return m_lives;
}

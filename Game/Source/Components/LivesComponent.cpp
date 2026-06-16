// Game
#include "Commands.hpp"
#include "Components/LivesComponent.hpp"
// Engine
#include "Engine/Scene/GameObject.hpp"
#include "Engine/InputManager.hpp"
// Standard
#include <print>

Game::LivesComponent::LivesComponent(Engine::GameObject& owner, uint32_t const lives) noexcept
    : Component(owner)
    , m_lives(lives)
    , m_renderComponent{
        *m_owner.GetComponent<Engine::RenderComponent>(),
    }
{
    Engine::InputManager& inputManager{ Engine::InputManager::GetInstance() };
    inputManager.Bind({SDL_SCANCODE_K, Engine::InputType::released}, std::make_unique<TakeDamageCommand>(*this));
    m_renderComponent.SetInstanceCount(m_lives);
}

void Game::LivesComponent::TakeDamage() noexcept
{
    if (!m_lives) return;// Corpses do not care about damage
    --m_lives;
    m_renderComponent.SetInstanceCount(m_lives);
    NotifyObservers(m_onDamageTaken);
    if (!m_lives)// Just died
    {
        NotifyObservers(m_onDiedEvent);
        return;
    }
    std::println("Took damage, lives: {}", m_lives);
}

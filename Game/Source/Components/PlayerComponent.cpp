// Game
#include "Components/PlayerComponent.h"
// Engine
#include "Commands.h"
#include "Engine/Components/MovementComponent.h"

Game::PlayerComponent::PlayerComponent(Engine::GameObject& owner) noexcept
    : Component(owner)
    , m_movementComponent(*owner.GetComponent<Engine::MovementComponent>())
{
    BindInput();
}

Game::PlayerComponent::~PlayerComponent() noexcept
{
    UnbindInput();
}

void Game::PlayerComponent::BindInput()
{
    Engine::InputManager& inputManager{ Engine::InputManager::GetInstance() };
    // Movement
    // Keyboard
    // They get unbound after the input manager is destroyed.
    // The player is destroyed when the scene is. It's lifetime is also static.
    // Why the fuck my scene is static at all?
    inputManager.Bind(m_upAction, std::make_unique<Engine::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, -1.f }));
    inputManager.Bind(m_leftAction, std::make_unique<Engine::MoveCommand>(m_movementComponent, glm::vec2{ -1.f, 0.f }));
    inputManager.Bind(m_downAction, std::make_unique<Engine::MoveCommand>(m_movementComponent, glm::vec2{ 0.f, 1.f }));
    inputManager.Bind(m_rightAction, std::make_unique<Engine::MoveCommand>(m_movementComponent, glm::vec2{ 1.f, 0.f }));
    inputManager.Bind(m_pointAction, std::make_unique<PointCommand>(*this));
    /// TODO: Gamepad
}

void Game::PlayerComponent::UnbindInput() const
{
    Engine::InputManager& inputManager{ Engine::InputManager::GetInstance() };
    inputManager.Unbind(m_upAction);
    inputManager.Unbind(m_leftAction);
    inputManager.Unbind(m_downAction);
    inputManager.Unbind(m_rightAction);
    inputManager.Unbind(m_pointAction);
}

void Game::PlayerComponent::OnNotify(Engine::Event const event, Engine::Subject const&) noexcept
{
    // TODO: Find a proper way to bind functions to the events directly
    switch (event.id)
    {
    case Engine::MakeSDBMHash("OnDied"):
        {
            m_owner.MarkForDeletion();
            break;
        }
    default: ;
    }
}

void Game::PlayerComponent::AddPoints(uint32_t const points) noexcept
{
    m_points += points;
    subject.NotifyObservers(m_onPointsIncreased);
    if (m_points == 5) subject.NotifyObservers(m_onCollected5Points);
}

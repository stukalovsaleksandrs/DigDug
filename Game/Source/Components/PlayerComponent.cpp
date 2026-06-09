// Game
#include "Components/PlayerComponent.hpp"
#include "Commands.hpp"
// Engine
#include "Engine/Components/MovementComponent.hpp"

Game::PlayerComponent::PlayerComponent(Engine::GameObject& owner, Dependencies const& dependencies) noexcept
    : PawnComponent{owner, dependencies}
    , m_playerStateMachine{{
        .animationComponent = *owner.GetComponent<Engine::AnimationComponent>(),
        .movementComponent = m_movementComponent,
        .owner = owner,
        .levelManager = m_dependencies.levelManager
    }}
{
    BindInput();
}

Game::PlayerComponent::~PlayerComponent() noexcept
{
    UnbindInput();
}

void Game::PlayerComponent::Update() noexcept
{
    PawnComponent::Update();
    m_playerStateMachine.Update();
}

void Game::PlayerComponent::BindInput() noexcept
{
    Engine::InputManager& inputManager{ Engine::InputManager::GetInstance() };
    // Movement

    //// Keyboard
    auto makeMoveCommand{ [this](glm::vec2 direction)
        {
            return std::make_unique<Engine::MoveCommand>(m_movementComponent, direction);
        }
    };

    inputManager.Bind(m_keyboardUp, makeMoveCommand(glm::vec2{ 0.f, -1.f }));
    inputManager.Bind(m_keyboardLeft, makeMoveCommand(glm::vec2{ -1.f, 0.f }));
    inputManager.Bind(m_keyboardDown, makeMoveCommand(glm::vec2{ 0.f, 1.f }));
    inputManager.Bind(m_keyboardRight, makeMoveCommand(glm::vec2{ 1.f, 0.f }));
    inputManager.Bind(m_keyboardPointAction, std::make_unique<PointCommand>(*this));

    // Gamepad
    inputManager.Bind(m_gamepadUp, makeMoveCommand(glm::vec2{ 0.f, -1.f }));
    inputManager.Bind(m_gamepadLeft, makeMoveCommand(glm::vec2{ -1.f, 0.f }));
    inputManager.Bind(m_gamepadDown, makeMoveCommand(glm::vec2{ 0.f, 1.f }));
    inputManager.Bind(m_gamepadRight, makeMoveCommand(glm::vec2{ 1.f, 0.f }));
}

void Game::PlayerComponent::UnbindInput() const noexcept
{
    Engine::InputManager& inputManager{ Engine::InputManager::GetInstance() };

    // TODO: RAII

    // Keyboard
    inputManager.Unbind(m_keyboardUp);
    inputManager.Unbind(m_keyboardLeft);
    inputManager.Unbind(m_keyboardDown);
    inputManager.Unbind(m_keyboardRight);
    inputManager.Unbind(m_keyboardPointAction);

    // Gamepad
    inputManager.Unbind(m_gamepadUp);
    inputManager.Unbind(m_gamepadLeft);
    inputManager.Unbind(m_gamepadDown);
    inputManager.Unbind(m_gamepadRight);
}

void Game::PlayerComponent::AddPoints(uint32_t const points) noexcept
{
    m_points += points;
    subject.NotifyObservers(m_onPointsIncreased);
    if (m_points == 5) subject.NotifyObservers(m_onCollected5Points);
}

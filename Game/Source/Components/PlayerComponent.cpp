// Game
#include "Components/PlayerComponent.h"
// Engine
#include "Commands.h"
#include "Engine/Components/MovementComponent.h"
#include "Engine/Utils/Constants.h"

Game::PlayerComponent::PlayerComponent(Engine::GameObject& owner) noexcept
    : Component(owner)
    , m_movementComponent(*owner.GetComponent<Engine::MovementComponent>())
    , m_renderComponent(*owner.GetComponent<Engine::RenderComponent>())
{
    BindInput();

    // Binding ourselves to movement component
    m_movementComponent.BindObserver(*this);
}

Game::PlayerComponent::~PlayerComponent() noexcept
{
    UnbindInput();
}

void Game::PlayerComponent::BindInput()
{
    Engine::InputManager& inputManager{ Engine::InputManager::GetInstance() };
    // Movement

    //// Keyboard
    auto makeMoveCommand{ [this](glm::vec2 direction)
        {
            return std::make_unique<Engine::MoveCommand>(m_movementComponent, direction);
        }
    };
    inputManager.Bind(m_upAction, makeMoveCommand(glm::vec2{ 0.f, -1.f }));
    inputManager.Bind(m_leftAction, makeMoveCommand(glm::vec2{ -1.f, 0.f }));
    inputManager.Bind(m_downAction, makeMoveCommand(glm::vec2{ 0.f, 1.f }));
    inputManager.Bind(m_rightAction, makeMoveCommand(glm::vec2{ 1.f, 0.f }));
    inputManager.Bind(m_pointAction, std::make_unique<PointCommand>(*this));
    //// TODO: Gamepad
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
    case std::to_underlying(EventType::OnDied):
        {
            m_owner.MarkForDeletion();
            break;
        }
    case std::to_underlying(Engine::EventType::OnDirectionChanged):
        {
            glm::vec2 const direction{ m_movementComponent.GetDirection() };
            SDL_FlipMode flipMode{};
            if (direction.x < 0.f) flipMode = SDL_FLIP_HORIZONTAL;
            m_renderComponent.SetFlipMode(flipMode);
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

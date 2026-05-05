// Game
#include "Components/PlayerComponent.h"
#include "Commands.h"
// Engine
#include "Engine/Components/MovementComponent.h"
#include "Engine/Utils/Constants.h"
#include "Engine/Utils/Timer.h"
// Third-party
#include "glm/geometric.hpp"
// Standard
#include <print>


Game::PlayerComponent::PlayerComponent(Engine::GameObject& owner, Dependencies const& dependencies) noexcept
    : Component{owner}
    , m_dependencies{ dependencies }
    , m_movementComponent{*owner.GetComponent<Engine::MovementComponent>()}
    , m_renderComponent{*owner.GetComponent<Engine::RenderComponent>()}
    , m_stateMachine{{
        .animationComponent = *owner.GetComponent<Engine::AnimationComponent>(),
        .movementComponent = m_movementComponent
    }}
{
    BindInput();

    // Binding ourselves to movement component
    m_movementComponent.BindObserver(*this);
}

Game::PlayerComponent::~PlayerComponent() noexcept
{
    UnbindInput();
}

void Game::PlayerComponent::Update() noexcept
{
    Component::Update();
    m_stateMachine.Update();
    if (auto const lerpedLocation{ m_locationLerpData.Update()};
        lerpedLocation != std::nullopt)
    {
        m_owner.SetLocalPosition(lerpedLocation.value());
    }
    m_movementComponent.Enable();
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
    inputManager.Bind(m_upAction, makeMoveCommand(glm::vec2{ 0.f, -1.f }));
    inputManager.Bind(m_leftAction, makeMoveCommand(glm::vec2{ -1.f, 0.f }));
    inputManager.Bind(m_downAction, makeMoveCommand(glm::vec2{ 0.f, 1.f }));
    inputManager.Bind(m_rightAction, makeMoveCommand(glm::vec2{ 1.f, 0.f }));
    inputManager.Bind(m_pointAction, std::make_unique<PointCommand>(*this));
    //// TODO: Gamepad
}

void Game::PlayerComponent::UnbindInput() const noexcept
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
            ProcessSpriteOrientation(direction);
            break;
        }
    case std::to_underlying(Engine::EventType::OnMovementAxisChanged):
        {
            ConstrainMovementToGrid();
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

void Game::PlayerComponent::ProcessSpriteOrientation(glm::vec2 const direction) const noexcept
{
    // Flipping
    SDL_FlipMode flipMode{};
    if (direction.x < 0.f) flipMode = SDL_FLIP_HORIZONTAL;
    m_renderComponent.SetFlipMode(flipMode);

    // Rotation
    if (direction.y < 0.f) m_renderComponent.SetRotation(-90.f);
    else if (direction.y > 0.f) m_renderComponent.SetRotation(90.f);
    else m_renderComponent.SetRotation(0.f);
}

void Game::PlayerComponent::ConstrainMovementToGrid() noexcept
{
    glm::vec2 const srcTopLeft{  m_owner.GetWorldPosition()  };// in px
    glm::vec2 const dstTopLeft{  GetCurrentCellTopLeft()  };// in px
    float const distance{ glm::length(dstTopLeft - m_owner.GetWorldPosition()) };// in px
    float const lerpSec{ distance / m_movementComponent.GetPxPerSec() };

    m_movementComponent.Disable();
    m_locationLerpData.Reset(srcTopLeft, dstTopLeft, lerpSec);
}

glm::vec2 Game::PlayerComponent::GetCurrentCellTopLeft() const noexcept
{
    glm::ivec2 const currentCell{ m_dependencies.grid.GetCellFromPoint(m_owner.GetWorldPosition() + 0.5f * glm::vec2{characterDims}) };
    return m_dependencies.grid.GetCellTopLeft(currentCell);
}

/************************
 * LocationLerpData
************************/

void Game::PlayerComponent::LocationLerpData::Reset(glm::vec2 const srcLocation, glm::vec2 const dstLocation, float const totalSec) noexcept
{
    if (Engine::Utils::NearlyEqual(srcLocation, dstLocation)) return;

    m_srcLocation = srcLocation;
    m_dstLocation = dstLocation;
    m_dstSec = totalSec;

    m_currentSec = 0.f;
    m_active = true;
}

std::optional<glm::vec2> Game::PlayerComponent::LocationLerpData::Update() noexcept
{
    if (!m_active) return std::nullopt;
    m_currentSec += Engine::Timer::GetInstance().GetDeltaSec();
    glm::vec2 const currentLocation{ m_srcLocation + (m_dstLocation - m_srcLocation) * (m_currentSec / m_dstSec) };
    // Disabling on overshoot
    if (glm::dot(m_dstLocation - m_srcLocation, m_dstLocation - currentLocation) < 0.f) m_active = false;

    return currentLocation;
}

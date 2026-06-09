
// Game
#include "Components/PawnComponent.hpp"
#include "Levels/LevelManager.hpp"
// Engine
#include "Engine/Components/MovementComponent.hpp"
#include "Engine/Components/Components.hpp"
#include "Engine/Scene/GameObject.hpp"
#include "Engine/Utils/Timer.hpp"
#include "Engine/Utils/Constants.hpp"
// Third-party
#include "glm/geometric.hpp"
#include "Levels/Level.hpp"
// Standard
#include <print>

Game::PawnComponent::PawnComponent(Engine::GameObject& owner, Dependencies const& dependencies) noexcept
    : Component{ owner }
    , m_dependencies{ dependencies }
    , m_movementComponent{*owner.GetComponent<Engine::MovementComponent>()}
    , m_renderComponent{*owner.GetComponent<Engine::RenderComponent>()}
{
    m_movementComponent.BindObserver(*this);
}

void Game::PawnComponent::OnNotify(Engine::Event const event, Engine::Subject const& caller) noexcept
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

void Game::PawnComponent::Update() noexcept
{
    Component::Update();
    if (auto const lerpedLocation{ m_locationLerpData.Update()};
        lerpedLocation != std::nullopt)
    {
        m_owner.SetLocalPosition(lerpedLocation.value());
    }
    m_movementComponent.Enable();
}

void Game::PawnComponent::ProcessSpriteOrientation(glm::vec2 const direction) const noexcept
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

void Game::PawnComponent::ConstrainMovementToGrid() noexcept
{
    glm::vec2 const srcTopLeft{  m_owner.GetWorldLocation()  };// in px
    glm::vec2 const dstTopLeft{  GetCurrentCellTopLeft()  };// in px
    float const distance{ glm::length(dstTopLeft - m_owner.GetWorldLocation()) };// in px
    float const lerpSec{ distance / m_movementComponent.GetPxPerSec() };

    m_movementComponent.Disable();
    m_locationLerpData.Reset(srcTopLeft, dstTopLeft, lerpSec);
}

glm::vec2 Game::PawnComponent::GetCurrentCellTopLeft() const noexcept
{
    Level const& level{ m_dependencies.levelManager.GetCurrentLevel() };
    return level.GetCellTopLeft(m_owner.GetWorldLocation() + 0.5f * glm::vec2{tileSideLength});
}

 #pragma region LocationLerpData
void Game::PawnComponent::LocationLerpData::Reset(glm::vec2 const srcLocation, glm::vec2 const dstLocation, float const totalSec) noexcept
{
    if (Engine::Utils::NearlyEqual(srcLocation, dstLocation)) return;

    m_srcLocation = srcLocation;
    m_dstLocation = dstLocation;
    m_dstSec = totalSec;

    m_currentSec = 0.f;
    m_active = true;
}

std::optional<glm::vec2> Game::PawnComponent::LocationLerpData::Update() noexcept
{
    if (!m_active) return std::nullopt;
    m_currentSec += Engine::Timer::GetInstance().GetDeltaSec();
    glm::vec2 const currentLocation{ m_srcLocation + (m_dstLocation - m_srcLocation) * (m_currentSec / m_dstSec) };
    // Disabling on overshoot
    if (glm::dot(m_dstLocation - m_srcLocation, m_dstLocation - currentLocation) < 0.f) m_active = false;

    return currentLocation;
}
#pragma endregion LocationLerpData

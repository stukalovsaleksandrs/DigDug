// Project
#include "Components/PointDisplayComponent.hpp"
#include "Components/PlayerComponent.hpp"
// Engine
#include "Engine/Scene/GameObject.hpp"
// Standard
#include <format>

Game::PointDisplayComponent::PointDisplayComponent(Engine::GameObject& owner, PlayerComponent& playerComponent) noexcept
    : Component(owner)
    , m_playerComponent(playerComponent)
    , m_textComponent(*owner.GetComponent<Engine::TextComponent>())
{
    m_textComponent.SetText(std::format("Points: {}", m_playerComponent.GetPoints()));
}

void Game::PointDisplayComponent::OnNotify(Engine::Event const event, Engine::Subject const& caller) noexcept
{
    switch (event.id)
    {
    case std::to_underlying(EventType::OnPointsIncreased):
        m_textComponent.SetText(std::format("Points: {}", m_playerComponent.GetPoints()));
        break;
    case std::to_underlying(Engine::CommonEvents::SubjectDeleted):
        std::erase(m_pSubjects, &caller);
        break;
    default: ;
    }
}

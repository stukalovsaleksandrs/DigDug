#include "Components/LivesDisplayComponent.hpp"
#include "Components/LivesComponent.hpp"
#include "Engine/Components/Components.hpp"
#include "Engine/Scene/GameObject.hpp"
// Standard
#include <format>


// TODO: For displaying sprites as lives, I can spawn a game object for every live

Game::LivesDisplayComponent::LivesDisplayComponent(Engine::GameObject& owner, LivesComponent const& livesComponent) noexcept
    : Component{ owner }
    , m_pLivesComponent{ &livesComponent }
    , m_pTextComponent{ m_owner.GetComponent<Engine::TextComponent>() }
{
    m_pTextComponent->SetText(std::format("Lives: {}", m_pLivesComponent->GetLives()));
}

void Game::LivesDisplayComponent::OnNotify(Engine::Event const event, Engine::Subject const& caller) noexcept
{
    switch (event.id)
    {
    case std::to_underlying(EventType::OnDamageTaken):
        {
            m_pTextComponent->SetText(std::format("Lives: {}", m_pLivesComponent->GetLives()));
            break;
        }
        case std::to_underlying(Engine::CommonEvents::SubjectDeleted):
        {
            std::erase(m_pSubjects, &caller);
            break;
        }
        default:
            break;
    }

}

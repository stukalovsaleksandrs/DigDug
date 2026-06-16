#include "Components/LivesDisplayComponent.hpp"
#include "Components/LivesComponent.hpp"
#include "Engine/Components/Components.hpp"
#include "Engine/Scene/GameObject.hpp"
// Standard
#include <format>


// TODO: For displaying sprites as lives, I can spawn a game object for every live
// Option 2. Make the render component accept the instance count

Game::LivesDisplayComponent::LivesDisplayComponent(Engine::GameObject& owner, LivesComponent const& livesComponent) noexcept
    : Component{ owner }
    , m_livesComponent{ livesComponent }
    , m_renderComponent{
        *m_owner.GetComponent<Engine::RenderComponent>(),
    }
{
    m_renderComponent.SetInstanceCount(m_livesComponent.GetLives());
}

void Game::LivesDisplayComponent::OnNotify(Engine::Event const event, Engine::Subject const& caller) noexcept
{
    switch (event.id)
    {
        case std::to_underlying(EventType::OnDamageTaken):
        {
            m_renderComponent.SetInstanceCount(m_livesComponent.GetLives());
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

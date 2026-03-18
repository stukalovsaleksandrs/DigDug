#include "Components/LivesDisplayComponent.h"
#include "Components/LivesComponent.h"
#include "Scene/GameObject.h"
#include <format>

// TODO: For displaying sprites as lives, I can spawn a game object for every live

DAE::Components::LivesDisplayComponent::LivesDisplayComponent(GameObject& owner, LivesComponent const& livesComponent) noexcept
    : Component(owner)
    , m_pLivesComponent(&livesComponent)
    , m_pTextComponent(m_owner.GetComponent<TextComponent>())
{
    m_pTextComponent->SetText(std::format("Lives: {}", m_pLivesComponent->GetLives()));
}

void DAE::Components::LivesDisplayComponent::OnNotify(Event const event, Subject const& subject) noexcept
{
    switch (event.id)
    {
        case MakeSDBMHash("OnDamageTaken"):
        {
            auto const lives{ m_pLivesComponent->GetLives() };
            m_pTextComponent->SetText(std::format("Lives: {}", lives));
            break;
        }
        case std::to_underlying(CommonEvents::SubjectDeleted):
        {
            std::erase(m_pSubjects, &subject);
        }
        default:
            break;
    }

}

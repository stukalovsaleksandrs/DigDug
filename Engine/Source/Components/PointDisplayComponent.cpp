#include "Components/PointDisplayComponent.h"

#include <format>

#include "Scene/GameObject.h"
#include "Components/PlayerComponent.h"

DAE::Components::PointDisplayComponent::PointDisplayComponent(GameObject& owner, PlayerComponent& playerComponent) noexcept
    : Component(owner)
    , m_playerComponent(playerComponent)
    , m_textComponent(*owner.GetComponent<TextComponent>())
{
    m_textComponent.SetText(std::format("Points: {}", m_playerComponent.GetPoints()));
}

void DAE::Components::PointDisplayComponent::OnNotify(Event const event, Subject const& caller) noexcept
{
    switch (event.id)
    {
    case MakeSDBMHash("OnPointsIncreased"):
        m_textComponent.SetText(std::format("Points: {}", m_playerComponent.GetPoints()));
        break;
    case std::to_underlying(CommonEvents::SubjectDeleted):
        std::erase(m_pSubjects, &caller);
        break;
    default: ;
    }
}

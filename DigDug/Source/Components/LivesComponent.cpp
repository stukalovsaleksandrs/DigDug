#include "Components/LivesComponent.h"
#include <print>

#include "Scene/GameObject.h"

DAE::Components::LivesComponent::LivesComponent(GameObject& owner, uint32_t const lives)
    : Component(owner)
    , m_lives(lives)
{}

void DAE::Components::LivesComponent::OnLivesChanged(int const deltaLives)
{
    if (m_lives += deltaLives) return;// Not dead yet
    // TODO: Fire onDiedEvent
    std::println("Player died");
    m_owner.MarkForDeletion();
}

#include "Components/LivesDisplayComponent.h"

#include "Components/LivesComponent.h"
#include "Scene/GameObject.h"


DAE::Components::LivesDisplayComponent::LivesDisplayComponent(LivesComponent const& livesComponent) noexcept
    : Component(livesComponent.GetOwner())
{}



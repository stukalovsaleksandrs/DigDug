#include "Input/Input.h"
#include "GameObject.h"
#include "Components/MovementComponent.h"

void DAE::Input::MoveLeftCommand::Execute(GameObject& gameObject) noexcept
{
    auto const pMovementComponent{ gameObject.GetComponent<Components::MovementComponent>() };
    if (!pMovementComponent) return;
    pMovementComponent->direction.x -= 1.f;
}

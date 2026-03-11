#include "Components/InputComponent.h"
#include "Timer.h"
#include "GameObject.h"
#include <SDL3/SDL_keyboard.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/norm.hpp"

DAE::Components::InputComponent::InputComponent(GameObject& owner) noexcept
    : Component(owner)
{}

void DAE::Components::InputComponent::Update() noexcept
{
    bool const* const pKeyboardState = SDL_GetKeyboardState(nullptr);
    glm::vec2 direction{};
    if (pKeyboardState[SDL_SCANCODE_A] || pKeyboardState[SDL_SCANCODE_LEFT]) {
        direction.x -= 1;
    }
    if (pKeyboardState[SDL_SCANCODE_D] || pKeyboardState[SDL_SCANCODE_RIGHT]) {
        direction.x += 1;
    }
    if (pKeyboardState[SDL_SCANCODE_S] || pKeyboardState[SDL_SCANCODE_DOWN]) {
        direction.y += 1;
    }
    if (pKeyboardState[SDL_SCANCODE_W] || pKeyboardState[SDL_SCANCODE_UP])
    {
        direction.y -= 1;
    }
    // Returning early if standing in-place to avoid division by 0
    if (glm::length2(direction) < glm::epsilon<float>()) return;

    static constexpr float pixelsPerSec{ 100.f };
    m_owner.SetLocalPosition(
        m_owner.GetLocalPosition() + glm::normalize(direction) * pixelsPerSec * Timer::GetInstance().GetDeltaSec()
    );
}

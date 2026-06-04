// Engine
#include "Utils/Timer.hpp"
#include "Components/MovementComponent.hpp"
#include "Utils/Constants.hpp"
#include "Engine/Components/AnimationComponent.hpp"
#include "Engine/Scene/GameObject.hpp"
// Standard
#include <ranges>
#include <iostream>

Engine::AnimationComponent::AnimationComponent(GameObject& owner, Data const& data) noexcept
    : Component{owner}
      , m_data{data}
      , m_ownerRenderComponent{
          m_owner.AddComponent<RenderComponent>(data.firstSpriteView)
      }
{
    SetFrameSpriteViews();
}

void Engine::AnimationComponent::Update() noexcept
{
    Component::Update();
    if (m_data.frameCount < 1 || Utils::NearlyZero(m_data.secPerFrame)) return;
    if (m_currentSec >= m_data.secPerFrame)
    {
        // Switching to the next frame
        ++m_currentFrameIdx %= m_data.frameCount;

        // Switching the sprite view in the render component
        m_ownerRenderComponent.SetSpriteView(m_frames.at(m_currentFrameIdx));

        // Resetting the timer
        // NOTE: Handling the case if window is dragged for instance
        while (m_currentSec >= m_data.secPerFrame)
        {
            m_currentSec -= m_data.secPerFrame;
        }
    }

    m_currentSec += Timer::GetInstance().GetDeltaSec();
}

void Engine::AnimationComponent::ChangeAnimation(SDL_FRect const srcRect, uint32_t const frameCount) noexcept
{
    m_data.firstSpriteView.srcRect = srcRect;
    m_data.frameCount = frameCount;
    SetFrameSpriteViews();
    m_ownerRenderComponent.SetSpriteView(m_data.firstSpriteView);
    m_currentFrameIdx = 0;
    m_currentSec = 0.f;
}

void Engine::AnimationComponent::SetFrameSpriteViews()
{
    m_frames.clear();
    m_frames.reserve(m_data.frameCount);
    for (auto const frameSpriteIdx : std::views::iota(0u, m_data.frameCount))
    {
        SDL_FRect const firstSpriteRect{ m_data.firstSpriteView.srcRect };
        SDL_FRect srcRect{ m_data.firstSpriteView.srcRect };
        srcRect.x = firstSpriteRect.x + firstSpriteRect.w * static_cast<float>(frameSpriteIdx);

        m_frames.emplace_back(
            m_data.firstSpriteView.pSprite,
            srcRect
        );
    }
}

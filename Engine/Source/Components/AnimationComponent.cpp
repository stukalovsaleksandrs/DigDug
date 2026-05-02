// Engine
#include "Utils/Timer.h"
#include "Utils/Constants.h"
#include "Engine/Components/AnimationComponent.h"
#include "Engine/Scene/GameObject.h"
// Standard
#include <ranges>

#include "Components/MovementComponent.h"

Engine::AnimationComponent::AnimationComponent(GameObject& owner, Data const& data) noexcept
    : Component{owner}
      , m_data{data}
      , m_ownerRenderComponent{
          m_owner.AddComponent<RenderComponent>(data.firstSpriteView)
      }
{
    InitializeFrameSpriteViews();
}

void Engine::AnimationComponent::Update() noexcept
{
    Component::Update();
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

void Engine::AnimationComponent::InitializeFrameSpriteViews()
{
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

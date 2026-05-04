#ifndef ENGINE_ANIMATION_COMPONENT
#define ENGINE_ANIMATION_COMPONENT

// Engine
#include "ComponentBase.h"
#include "Engine/Rendering/Sprite.h"
// Third-party
#include <SDL3/SDL_rect.h>

namespace Engine
{
    class RenderComponent;

    // WARNING: Assumes that all the frames are laid out horizontally
    class AnimationComponent final : public Component
    {
    public:
        // Reuses the dimensions from srcRect for all frames
        struct Data final
        {
            Sprite::View firstSpriteView;
            uint32_t frameCount;
            float secPerFrame;
        };
        explicit AnimationComponent(GameObject& owner, Data const&) noexcept;
        void Update() noexcept override;
        void ChangeAnimation(SDL_FRect srcRect, uint32_t frameCount) noexcept;

    private:
        Data m_data;
        uint32_t m_currentFrameIdx{};
        float m_currentSec{};
        RenderComponent& m_ownerRenderComponent;
        std::vector<Sprite::View> m_frames;

        void SetFrameSpriteViews();
    };
}

#endif //ENGINE_ANIMATION_COMPONENT

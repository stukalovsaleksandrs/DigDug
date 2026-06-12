#ifndef ENGINE_ANIMATION_COMPONENT
#define ENGINE_ANIMATION_COMPONENT

// Engine
#include "ComponentBase.hpp"
#include "Engine/Rendering/Sprite.hpp"
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
        void ChangeSource(SDL_FRect srcRect, uint32_t frameCount, float secPerFrame) noexcept;
        void ChangeAnimation(Data const& data) noexcept;;

    private:
        Data m_data;
        uint32_t m_currentFrameIdx{};
        float m_currentSec{};
        RenderComponent& m_ownerRenderComponent;
        std::vector<Sprite::View> m_frames;

        void SetSpriteViews();
    };
}

#endif //ENGINE_ANIMATION_COMPONENT

#ifndef GAME_PUMP_COMPONENT
#define GAME_PUMP_COMPONENT

// Engine
#include "Engine/Components/ComponentBase.hpp"
#include "Engine/Rendering/Sprite.hpp"

namespace Engine
{
    class RenderComponent;
}

namespace Game
{
    class PumpComponent final : public Engine::Component
    {
    public:
        explicit PumpComponent(Engine::GameObject& owner, Engine::RenderComponent const & playerRenderComponent,
            Engine::Sprite & pumpHorizontalSprite, Engine::Sprite & pumpVerticalSprite);

        void Update() noexcept override;

    private:
        Engine::RenderComponent& m_renderComponent;
        Engine::RenderComponent const& m_playerRenderComponent;
        Engine::Sprite & m_pumpHorizontalSprite, & m_pumpVerticalSprite;
        float const m_pxPerSec{ 100.f };
        float const m_maxWidthPx{};
        float m_currentWidthPx{};
        float m_currentDegrees{-90.f};

        void OnEnable() noexcept override;
        void OnDisable() noexcept override;

    };

}

#endif// GAME_PUMP_COMPONENT

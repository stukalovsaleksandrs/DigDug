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
    class Level;

    class PumpComponent final : public Engine::Component
    {
    public:
        explicit PumpComponent(
            Engine::GameObject& owner,
            Engine::RenderComponent const& playerRenderComponent,
            Level const&
        );

        void Update() noexcept override;

    private:
        Engine::RenderComponent& m_renderComponent;
        Engine::RenderComponent const& m_playerRenderComponent;
        Level const& m_level;
        float const m_pxPerSec{ 100.f };
        float const m_maxWidthPx{};
        float m_currentWidthPx{};

        void OnEnable() noexcept override;
        void OnDisable() noexcept override;

    };

}

#endif// GAME_PUMP_COMPONENT

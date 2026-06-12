#ifndef GAME_PUMP_COMPONENT
#define GAME_PUMP_COMPONENT

// Engine
#include "Utils.hpp"
#include "Engine/Components/ComponentBase.hpp"
#include "Engine/Components/Components.hpp"
#include "Engine/Core/Observer.hpp"
#include "Engine/Rendering/Sprite.hpp"
#include "Engine/Scene/GameObject.hpp"

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
            Level&
        );

        void Update() noexcept override;

        // How long will the pump take to play the entire animation
        void SetDuration(float const seconds)
        {
            m_pxPerSec = m_maxWidthPx / seconds;
        }

        [[nodiscard]] SDL_FRect GetDstRect() const noexcept
        {
            auto const topLeft{ m_owner.GetWorldLocation() };
            return SDL_FRect{
                topLeft.x, topLeft.y,
                m_renderComponent.dstDims.x,
                m_renderComponent.dstDims.y
            };
        }

        // Same as SetActive, but does not call callbacks
        void SetPaused(bool const paused) noexcept{ m_paused = paused; }

    private:
        Engine::RenderComponent& m_renderComponent;
        Engine::RenderComponent const& m_playerRenderComponent;
        Level& m_level;
        float const m_maxWidthPx{};
        float m_pxPerSec{}, m_currentWidthPx{};
        bool m_paused{};

        std::vector<Engine::RenderComponent*> m_enemyRenderComponents{};

        void OnEnable() noexcept override;
        void OnDisable() noexcept override;
    };

}

#endif// GAME_PUMP_COMPONENT

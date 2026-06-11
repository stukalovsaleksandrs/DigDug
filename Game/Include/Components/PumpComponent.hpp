#ifndef GAME_PUMP_COMPONENT
#define GAME_PUMP_COMPONENT

// Engine
#include "Engine/Components/ComponentBase.hpp"

namespace Engine
{
    class RenderComponent;
}

namespace Game
{
    class PumpComponent final : public Engine::Component
    {
    public:
        explicit PumpComponent(Engine::GameObject& owner);

        void Update() noexcept override;

    private:
        Engine::RenderComponent& m_renderComponent;
        float const m_lifetimeSec{ 1.f }, m_pxPerSec{ 50.f };
        float const m_maxWidthPx{};
        float m_currentWidthPx{};
    };


}

#endif// GAME_PUMP_COMPONENT

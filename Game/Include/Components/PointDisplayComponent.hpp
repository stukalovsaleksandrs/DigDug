#ifndef POINT_DISPLAY_COMPONENT
#define POINT_DISPLAY_COMPONENT

#include "Engine/Components/ComponentBase.hpp"
#include "Engine/Core/Observer.hpp"

namespace Engine
{
    class TextComponent;
}

namespace Game
{
    class PlayerComponent;

    class PointDisplayComponent : public Engine::Component, public Engine::Observer
    {
    public:
        explicit PointDisplayComponent(Engine::GameObject& owner, PlayerComponent& playerComponent) noexcept;
        void OnNotify(Engine::Event event, Engine::Subject const& caller) noexcept override;

    private:
        PlayerComponent const& m_playerComponent;
        Engine::TextComponent& m_textComponent;

    };
}

#endif

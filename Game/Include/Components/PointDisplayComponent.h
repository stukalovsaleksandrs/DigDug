#ifndef __POINT_DISPLAYCOMPONENT_H__
#define __POINT_DISPLAYCOMPONENT_H__

#include "Engine/Components/ComponentBase.h"
#include "Engine/Core/Observer.h"

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

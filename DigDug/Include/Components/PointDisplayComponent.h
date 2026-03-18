#ifndef __POINT_DISPLAYCOMPONENT_H__
#define __POINT_DISPLAYCOMPONENT_H__

#include "ComponentBase.h"
#include "Core/Observer.h"

namespace DAE::Components
{
    class PlayerComponent;
    class TextComponent;

    class PointDisplayComponent : public Component, public Observer
    {
    public:
        explicit PointDisplayComponent(GameObject& owner, PlayerComponent& playerComponent) noexcept;
        void OnNotify(Event event, Subject const& caller) noexcept override;

    private:
        PlayerComponent const& m_playerComponent;
        TextComponent& m_textComponent;

    };
}

#endif

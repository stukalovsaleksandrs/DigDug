#ifndef __LIVESDISPLAYCOMPONENT_H__
#define __LIVESDISPLAYCOMPONENT_H__

#include "../../../Engine/Include/Engine/Components/ComponentBase.h"
#include "Engine/Core/Observer.h"

namespace DAE::Components
{
    class LivesComponent;
    class TextComponent;

    // Requires the owner to gave LivesComponent
    class LivesDisplayComponent : public Component, public Observer
    {
    public:
        explicit LivesDisplayComponent(GameObject& owner, LivesComponent const&) noexcept;
        void OnNotify(Event event, Subject const& caller) noexcept override;

    private:
        LivesComponent const* m_pLivesComponent;
        TextComponent* m_pTextComponent;

    };
}

#endif

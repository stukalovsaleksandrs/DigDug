#ifndef __LIVESDISPLAYCOMPONENT_H__
#define __LIVESDISPLAYCOMPONENT_H__

// ENGINE
#include "Engine/Components/ComponentBase.h"
#include "Engine/Core/Observer.h"

namespace Engine
{
    class TextComponent;
}

namespace Game
{
    class LivesComponent;

    // Requires the owner to gave LivesComponent
    class LivesDisplayComponent : public Engine::Component, public Engine::Observer
    {
    public:
        explicit LivesDisplayComponent(Engine::GameObject& owner, LivesComponent const&) noexcept;
        void OnNotify(Engine::Event event, Engine::Subject const& caller) noexcept override;

    private:
        LivesComponent const* m_pLivesComponent;
        Engine::TextComponent* m_pTextComponent;

    };
}

#endif

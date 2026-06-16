#ifndef __LIVESDISPLAYCOMPONENT_H__
#define __LIVESDISPLAYCOMPONENT_H__

// ENGINE
#include "Engine/Components/ComponentBase.hpp"
#include "Engine/Components/Components.hpp"
#include "Engine/Core/Observer.hpp"

namespace Engine
{
    class TextComponent;
    class RenderComponent;
}

namespace Game
{
    class LivesComponent;

    // Requires the owner to gave LivesComponent
    class LivesDisplayComponent final : public Engine::Component, public Engine::Observer
    {
    public:
        // NOTE: LivesComponent likely does not belong to the owner
        explicit LivesDisplayComponent(Engine::GameObject& owner, LivesComponent const&) noexcept;
        void OnNotify(Engine::Event event, Engine::Subject const& caller) noexcept override;

    private:
        LivesComponent const& m_livesComponent;
        Engine::RenderComponent& m_renderComponent;

    };
}

#endif

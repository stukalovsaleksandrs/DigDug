#ifndef __LIVESDISPLAYCOMPONENT_H__
#define __LIVESDISPLAYCOMPONENT_H__
#include <memory>

#include "ComponentBase.h"

namespace DAE::Components
{
    class LivesComponent;

    // Requires the owner to gave LivesComponent
    class LivesDisplayComponent : Component
    {
    public:
        explicit LivesDisplayComponent(LivesComponent const&) noexcept;

        // So, I want to subscribe it to onDamageTaken too, and
        // the LivesDisplayComponent will update the rendered element count
        // It somehow has to change the amount of times the sprite is rendered,
        // but my RenderComponent renders only once.
        // I potentially should create another RenderComponent with varying
        // sprite repetition times

        // So, I start creating multiple RenderComponents
        // 1. They all have a Texture2D to render
        // 2. They are all registered to the Renderer
        // Problem is that the render component accepts a RenderComponent
        // I should make a common interface for that, how will I call it, IRenderComponent?
        // Technically I could accept any class that has Render method..

    private:

    };
}

#endif

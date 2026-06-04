#ifndef DIGDUG_DIGGINGCOMPONENT_H
#define DIGDUG_DIGGINGCOMPONENT_H

// Engine
#include "Engine/Components/ComponentBase.hpp"

namespace Game
{
    class DiggingComponent final : public Engine::Component
    {
    public:
        explicit DiggingComponent(Engine::GameObject& owner) noexcept;
        ~DiggingComponent() noexcept override;
        void Render() const noexcept;

    private:
        std::function<void()> m_renderFunction{ [this]{this->Render();} };

    };
}

#endif //DIGDUG_DIGGINGCOMPONENT_H

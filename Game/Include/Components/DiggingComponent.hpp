#ifndef GAME_DIGGING_COMPONENT
#define GAME_DIGGING_COMPONENT

// Engine
#include "Engine/Components/ComponentBase.hpp"
// Third-party
#include <SDL3/SDL_render.h>

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

        SDL_Texture* m_maskTexture{};
        SDL_Renderer* m_pSDLRenderer{};

        void ApplyMask() const noexcept;
    };
}

#endif// GAME_DIGGING_COMPONENT

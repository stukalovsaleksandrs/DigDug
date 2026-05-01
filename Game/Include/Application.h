#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

// Engine
#include "Engine/Core/Application.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Rendering/Font.h"
#include "Engine/Rendering/Texture2D.h"

namespace Game
{
    class Application final : public Engine::Application
    {
    public:
        Application();

    protected:
        void Update() override;

    private:
        Engine::Scene scene;
        std::unique_ptr<Engine::Font> m_pFont{};

        std::unique_ptr<Engine::Texture2D> m_pBackgroundTexture{};
        std::unique_ptr<Engine::Texture2D> m_pSpriteSheet{};
    };
}
#endif// GAME_APPLICATION_H

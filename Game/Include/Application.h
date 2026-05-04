#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

// Game
#include "Grid.h"
// Engine
#include "Engine/Core/Application.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Rendering/Font.h"
#include "Engine/Rendering/Sprite.h"

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

        std::unique_ptr<Engine::Sprite> m_pBackgroundTexture{};
        std::unique_ptr<Engine::Sprite> m_pSpriteSheet{};

        std::unique_ptr<Grid> m_grid;
    };
}
#endif// GAME_APPLICATION_H

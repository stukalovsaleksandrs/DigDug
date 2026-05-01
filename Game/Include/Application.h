#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

// Engine
#include "Engine/Core/Application.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Rendering/Font.h"

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

    };
}
#endif// GAME_APPLICATION_H

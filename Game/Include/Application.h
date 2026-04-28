#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H

// Engine
#include "Engine/Core/Application.h"
#include "Engine/Scene/Scene.h"

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

    };
}
#endif// GAME_APPLICATION_H

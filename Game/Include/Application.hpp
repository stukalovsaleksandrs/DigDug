#ifndef GAME_APPLICATION
#define GAME_APPLICATION

// Game
#include "Levels/LevelManager.hpp"
// Engine
#include "Engine/Core/Application.hpp"
#include "Engine/Scene/Scene.hpp"
#include "Engine/Rendering/Font.hpp"
#include "Engine/Rendering/Sprite.hpp"

namespace Game
{
    class Application final : public Engine::Application
    {
    public:
        Application();

    protected:
        void Update() override;

    private:
        std::unique_ptr<Engine::Font> m_pFont{};

        std::unique_ptr<Engine::Sprite> m_pGroundSprite{};
        std::unique_ptr<Engine::Sprite> m_pSkySprite{};
        std::unique_ptr<Engine::Sprite> m_pTaizoHoriSprite{};

        std::unique_ptr<LevelManager> m_pLevelManager{};
    };
}
#endif// GAME_APPLICATION

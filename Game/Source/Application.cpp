// Project
#include "Constants.h"
#include "Application.h"
#include "Components/PlayerComponent.h"
#include "Components/LivesComponent.h"
#include "Components/LivesDisplayComponent.h"
#include "Components/PointDisplayComponent.h"
// Engine
#include "Engine/Scene/Scene.h"
#include "Engine/Components/MovementComponent.h"
#include "Engine/Rendering/Font.h"
#include "Engine/Rendering/Texture2D.h"
// Standard
#include <filesystem>

namespace fs = std::filesystem;

[[nodiscard]] std::string GetResourceFolderPath()
{
#if __EMSCRIPTEN__
    fs::path resourceFolderLocation = "";
#else
    auto resourceFolderName{"Resources"};
    fs::path resourceFolderLocation = std::format("./{}/", resourceFolderName);
    if (!fs::exists(resourceFolderLocation))
        resourceFolderLocation = std::format("../{}/", resourceFolderName);
#endif

    return resourceFolderLocation.generic_string();
}

Game::Application::Application()
    : Engine::Application(GetResourceFolderPath(), windowData, "Dig Dug")
{
    // cd into the resource directory
    fs::current_path(GetResourceFolderPath());
    m_pFont = std::make_unique<Engine::Font>("Lingua.otf", 36);
    m_pSpriteSheet = std::make_unique<Engine::Texture2D>("Sprites/DigDugSpriteSheet.png");
    m_pBackgroundTexture = std::make_unique<Engine::Texture2D>("Sprites/DigDugBackground.png");

    // Background
    {
        auto& background{scene.CreateGameObject({})};
        background.AddComponent<Engine::RenderComponent>(m_pBackgroundTexture.get());
    }

    // Character
    {
        auto& character{scene.CreateGameObject(glm::vec2{})};

        // TODO: Provide sprite sheet only once
        auto& characterRenderComponent{character.AddComponent<Engine::RenderComponent>(m_pSpriteSheet.get())};
        characterRenderComponent.SetTexture(m_pSpriteSheet.get(), SDL_FRect{0.f, 0.f,
            static_cast<float>(spriteDims.x), static_cast<float>(spriteDims.y)});

        character.AddComponent<Engine::MovementComponent>(75.f);
        auto& playerComponent{character.AddComponent<PlayerComponent>()};

        auto& livesComponent{character.AddComponent<LivesComponent>(2)};
        livesComponent.subject.BindObserver(playerComponent);

        // Lives display
        auto& livesDisplay{scene.CreateGameObject(glm::vec2{10.f, windowData.dims.y - 50.f})};
        livesDisplay.AddComponent<Engine::TextComponent>(" ", m_pFont.get()); // NOTE: Text must not be empty
        auto& livesDisplayComponent{livesDisplay.AddComponent<Game::LivesDisplayComponent>(livesComponent)};
        livesComponent.subject.BindObserver(livesDisplayComponent);

        // Point display
        auto& pointDisplay{scene.CreateGameObject(glm::vec2{10.f, windowData.dims.y - 100.f})};
        pointDisplay.AddComponent<Engine::TextComponent>("Points ", m_pFont.get());
        auto& pointDisplayComponent{pointDisplay.AddComponent<Game::PointDisplayComponent>(playerComponent)};
        playerComponent.subject.BindObserver(pointDisplayComponent);
    }
}

void Game::Application::Update()
{
    scene.Update();
}

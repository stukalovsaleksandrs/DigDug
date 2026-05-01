// Project
#include "Constants.h"
#include "Application.h"
#include "Components/PlayerComponent.h"
#include "Components/LivesComponent.h"
#include "Components/LivesDisplayComponent.h"
#include "Components/PointDisplayComponent.h"
// Engine
#include "Engine/Core/ResourceManager.h"
#include "Engine/Scene/Scene.h"
#include "Engine/Components/MovementComponent.h"
// Standard
#include <filesystem>
namespace fs = std::filesystem;

[[nodiscard]] std::string GetResourceFolderPath()
{
#if __EMSCRIPTEN__
    fs::path resourceFolderLocation  = "";
#else
    auto resourceFolderName{ "Resources" };
    fs::path resourceFolderLocation = std::format("./{}/", resourceFolderName);
    if(!fs::exists(resourceFolderLocation))
        resourceFolderLocation = std::format("../{}/", resourceFolderName);
#endif

    return resourceFolderLocation.generic_string();
}

Game::Application::Application()
    : Engine::Application(GetResourceFolderPath(), windowData, "Dig Dug")
{
    // Background
    {
        auto& background{ scene.CreateGameObject({}) };
        auto& backgroundRenderComponent{ background.AddComponent<Engine::RenderComponent>() };
        backgroundRenderComponent.SetTexture("DigDugBackground.png");
    }

    // Sprite sheet
    auto const pSpriteSheet{ Engine::ResourceManager::GetInstance().LoadTexture("DigDugSpriteSheet.png") };

    // Character
    {
        auto& character{ scene.CreateGameObject(glm::vec2{}) };

        // auto& characterRenderComponent{ character.AddComponent<Engine::RenderComponent>() };
        // characterRenderComponent.SetTexture(pSpriteSheet, {{}, spriteDims});

        character.AddComponent<Engine::MovementComponent>(500.f);
        auto& playerComponent{ character.AddComponent<Game::PlayerComponent>()};

        auto& livesComponent{ character.AddComponent<Game::LivesComponent>(2) };
        livesComponent.subject.BindObserver(playerComponent);

        // Lives display
        auto& livesDisplay{ scene.CreateGameObject(glm::vec2{10.f, windowData.dims.y - 50.f}) };
        auto const& pFont{ Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36)};
        livesDisplay.AddComponent<Engine::TextComponent>(" ", pFont);// NOTE: Text must not be empty
        auto& livesDisplayComponent{ livesDisplay.AddComponent<Game::LivesDisplayComponent>(livesComponent) };
        livesComponent.subject.BindObserver(livesDisplayComponent);

        // Point display
        auto& pointDisplay{ scene.CreateGameObject(glm::vec2{10.f, windowData.dims.y - 100.f}) };
        pointDisplay.AddComponent<Engine::TextComponent>("Points ", pFont);
        auto& pointDisplayComponent{ pointDisplay.AddComponent<Game::PointDisplayComponent>(playerComponent) };
        playerComponent.subject.BindObserver(pointDisplayComponent);
    }
}

void Game::Application::Update()
{
    scene.Update();
}

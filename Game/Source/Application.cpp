// Project
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

float constexpr g_resolutionScale{ 3 };
glm::vec2 constexpr g_logicalWindowDims{ 224.f, 288.f },
    g_windowDims{ g_logicalWindowDims * g_resolutionScale };

Game::Application::Application()
    : Engine::Application(GetResourceFolderPath(), g_windowDims, g_logicalWindowDims, "Dig Dug")
{
    // Background
    {
        auto& background{ scene.CreateGameObject({}) };
        auto& backgroundRenderComponent{ background.AddComponent<Engine::RenderComponent>() };
        backgroundRenderComponent.SetTexture("DigDugBackground.png");
    }

    // Sprite sheet
    auto spriteSheet{ Engine::ResourceManager::GetInstance().LoadTexture("DigDugSpriteSheet.png") };

    // Character
    {
        auto& character{ scene.CreateGameObject(glm::vec2{}) };

        auto& characterRenderComponent{ character.AddComponent<Engine::RenderComponent>() };
        characterRenderComponent.SetTexture("DigDugCharacter.png");

        character.AddComponent<Engine::MovementComponent>(500.f);
        auto& playerComponent{ character.AddComponent<Game::PlayerComponent>()};

        auto& livesComponent{ character.AddComponent<Game::LivesComponent>(2) };
        livesComponent.subject.BindObserver(playerComponent);

        // Lives display
        auto& livesDisplay{ scene.CreateGameObject(glm::vec2{10.f, g_windowDims.y - 50.f}) };
        auto const& pFont{ Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36)};
        livesDisplay.AddComponent<Engine::TextComponent>(" ", pFont);// NOTE: Text must not be empty
        auto& livesDisplayComponent{ livesDisplay.AddComponent<Game::LivesDisplayComponent>(livesComponent) };
        livesComponent.subject.BindObserver(livesDisplayComponent);

        // Point display
        auto& pointDisplay{ scene.CreateGameObject(glm::vec2{10.f, g_windowDims.y - 100.f}) };
        pointDisplay.AddComponent<Engine::TextComponent>("Points ", pFont);
        auto& pointDisplayComponent{ pointDisplay.AddComponent<Game::PointDisplayComponent>(playerComponent) };
        playerComponent.subject.BindObserver(pointDisplayComponent);
    }
}

void Game::Application::Update()
{
    scene.Update();
}

// My character does not move and does not get destroyed. Why?
// The movement action gets executed.
// But it never gets updated. I have to update the scene.


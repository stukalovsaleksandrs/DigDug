#include "Components/CacheThrashingComponent.h"
#include "Components/LivesComponent.h"
#include "Components/MovementComponent.h"
#include "Components/PlayerComponent.h"
#include "SDL3/SDL_main.h"// Required for the windows build not to give errors
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif
#include "Core/Application.h"
#include "Scene/SceneManager.h"
#include "Core/ResourceManager.h"
#include "Scene/Scene.h"
#include <filesystem>
#include <glm/glm.hpp>
namespace fs = std::filesystem;

static void Load()
{
    auto& scene{ DAE::SceneManager::GetInstance().CreateScene() };

    // Background
    auto pGameObject{ scene.CreateGameObject(glm::vec2{}) };
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject).SetTexture("Background.png");

    // Logo
    pGameObject = scene.CreateGameObject(glm::vec2{358, 180});
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject).SetTexture("Logo.png");

    // Title
    pGameObject = scene.CreateGameObject(glm::vec2{ 292, 20 });
    auto const& pFont{ DAE::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
    pGameObject->AddComponent<DAE::Components::TextComponent>(
        *pGameObject,
        "Programming 4 Assignment",
        pFont
    );

    // FPS
    pGameObject = scene.CreateGameObject(glm::vec2{ 10, 10 });
    pGameObject->AddComponent<DAE::Components::FPSComponent>(*pGameObject, pFont);

    // Character
    pGameObject = scene.CreateGameObject(glm::vec2{ 500, 250 });
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject).SetTexture("DigDugCharacter.png");
    auto& playerComponent{ pGameObject->AddComponent<DAE::Components::PlayerComponent>(
        pGameObject->AddComponent<DAE::Components::MovementComponent>(*pGameObject, 500.f)
    )};
    auto& livesComponent{ pGameObject->AddComponent<DAE::Components::LivesComponent>(*pGameObject, 2) };
    livesComponent.AddObserver(playerComponent);

    // Lives
    pGameObject = scene.CreateGameObject(glm::vec2{ 0, 576});
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject).SetTexture("DigDugCharacter.png");

}

int main(int, char*[]) {
#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    fs::path data_location = "./Data/";
    if(!fs::exists(data_location))
        data_location = "../Data/";
#endif
    DAE::Application game(data_location);
    game.Run(Load);
    return 0;
}

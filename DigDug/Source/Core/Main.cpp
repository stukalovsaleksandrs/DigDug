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

float constexpr resolutionScale{ 3 };
glm::vec2 constexpr originalGameResolution{ 224.f, 288.f },
    windowResolution{ originalGameResolution * resolutionScale };

static void Load()
{
    auto& scene{ DAE::SceneManager::GetInstance().CreateScene() };

    // FPS
    auto* pFPS{ scene.CreateGameObject(glm::vec2{ 10, 10 }) };
    auto const& pFont{ DAE::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
    pFPS->AddComponent<DAE::Components::FPSComponent>(*pFPS, pFont);

    // Character
    auto* pCharacter{ scene.CreateGameObject(glm::vec2{ 500, 250 }) };
    auto& characterRenderComponent{ pCharacter->AddComponent<DAE::Components::RenderComponent>(*pCharacter) };
    characterRenderComponent.SetTexture("DigDugCharacter.png");
    auto& playerComponent{ pCharacter->AddComponent<DAE::Components::PlayerComponent>(
        pCharacter->AddComponent<DAE::Components::MovementComponent>(*pCharacter, 500.f)
    )};
    auto& livesComponent{ pCharacter->AddComponent<DAE::Components::LivesComponent>(*pCharacter, 2) };
    livesComponent.AddObserver(playerComponent);

    // Lives
    pCharacter = scene.CreateGameObject(glm::vec2{ 0, windowResolution.y - characterRenderComponent.GetTextureDims().y });
    pCharacter->AddComponent<DAE::Components::RenderComponent>(*pCharacter).SetTexture("DigDugCharacter.png");
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    fs::path data_location = "./Data/";
    if(!fs::exists(data_location))
        data_location = "../Data/";
#endif
    DAE::Application game(data_location, windowResolution);
    game.Run(Load);
    return 0;
}

#include "Components/CacheThrashingComponent.h"
#include "Components/LivesComponent.h"
#include "Components/LivesDisplayComponent.h"
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

    // Character
    auto& character{ scene.CreateGameObject(glm::vec2{ 500.f, 250.f }) };

    auto& characterRenderComponent{ character.AddComponent<DAE::Components::RenderComponent>() };
    characterRenderComponent.SetTexture("DigDugCharacter.png");

    character.AddComponent<DAE::Components::MovementComponent>(500.f);
    auto& playerComponent{ character.AddComponent<DAE::Components::PlayerComponent>()};

    auto& livesComponent{ character.AddComponent<DAE::Components::LivesComponent>(2) };
    livesComponent.subject.BindObserver(playerComponent);

    // Lives display
    auto& livesDisplay{ scene.CreateGameObject(glm::vec2{10.f, windowResolution.y - 50.f}) };
    auto const& pFont{ DAE::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36)};
    livesDisplay.AddComponent<DAE::Components::TextComponent>(" ", pFont);// NOTE: Text must not be empty
    auto& livesDisplayComponent{ livesDisplay.AddComponent<DAE::Components::LivesDisplayComponent>(livesComponent) };
    livesComponent.subject.BindObserver(livesDisplayComponent);

    // Tutorial
    auto& tutorial{ scene.CreateGameObject(glm::vec2{10.f, 0.1f * windowResolution.y }) };
    auto const& pTutorialFont{ DAE::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20)};
    tutorial.AddComponent<DAE::Components::TextComponent>("Use WASD to move Dig Dug, K to inflict damage", pTutorialFont);

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

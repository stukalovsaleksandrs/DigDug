#include "SDL3/SDL_main.h"// Required for the windows build not to give errors
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif
#include "Application.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include <filesystem>
namespace fs = std::filesystem;

static void Load()
{
    auto& scene{ DAE::SceneManager::GetInstance().CreateScene() };

    // Background
    auto pGameObject{ std::make_unique<DAE::GameObject>() };
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject.get())->SetTexture("Background.png");
    scene.Add(std::move(pGameObject));

    // Logo
    pGameObject = std::make_unique<DAE::GameObject>();
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject.get())->SetTexture("Logo.png");
    pGameObject->AddComponent<DAE::Components::TransformComponent>(*pGameObject.get())->SetLocation({358, 180});
    scene.Add(std::move(pGameObject));

    // Title
    pGameObject = std::make_unique<DAE::GameObject>();
    pGameObject->AddComponent<DAE::Components::TransformComponent>(*pGameObject.get())->SetLocation({ 292, 20 });
    auto const& pFont{ DAE::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
    pGameObject->AddComponent<DAE::Components::TextComponent>(
        *pGameObject.get(),
        "Programming 4 Assignment",
        pFont
        );
    scene.Add(std::move(pGameObject));

    // FPS
    pGameObject = std::make_unique<DAE::GameObject>();
    pGameObject->AddComponent<DAE::Components::TransformComponent>(*pGameObject.get())->SetLocation({ 10, 10 });
    pGameObject->AddComponent<DAE::Components::FPSComponent>(*pGameObject.get(), pFont);
    scene.Add(std::move(pGameObject));
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

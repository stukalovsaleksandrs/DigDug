#include <SDL3/SDL.h>

#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif

#include "Minigin.h"
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
    pGameObject->SetTexture("Background.png");
    auto transformComponent{
        pGameObject->AddComponent<DAE::Components::TransformComponent>()
    };
    assert(transformComponent.has_value());
    transformComponent.value()->SetLocation({});
    scene.Add(std::move(pGameObject));

    // Logo
    pGameObject = std::make_unique<DAE::GameObject>();
    pGameObject->SetTexture("Logo.png");
    transformComponent = pGameObject->AddComponent<DAE::Components::TransformComponent>();
    assert(transformComponent.has_value());
    transformComponent.value()->SetLocation({358, 180});
    scene.Add(std::move(pGameObject));

    // auto font { DAE::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
    // auto pTextObject{ std::make_unique<DAE::TextObject>("Programming 4 Assignment", font) };
    // pTextObject->SetColor({ 255, 255, 0, 255 });
    // pTextObject->SetLocation({292, 20});
    // scene.Add(std::move(pTextObject));
}

int main(int, char*[]) {
#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    fs::path data_location = "./Data/";
    if(!fs::exists(data_location))
        data_location = "../Data/";
#endif
    DAE::Minigin engine(data_location);
    engine.Run(Load);
    return 0;
}

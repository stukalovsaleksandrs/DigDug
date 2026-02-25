#include <iostream>
#include "SDL3/SDL_main.h"// Required for the windows build not to give errors
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif
#include "Application.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include <filesystem>
#include <glm/glm.hpp>
namespace fs = std::filesystem;

static void Load()
{
    auto& scene{ DAE::SceneManager::GetInstance().CreateScene() };

    // Background
    auto pGameObject{ std::make_unique<DAE::GameObject>(glm::vec2{}) };
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject.get())->SetTexture("Background.png");

    scene.Add(std::move(pGameObject));

    // Logo
    pGameObject = std::make_unique<DAE::GameObject>(glm::vec2{358, 180});
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject.get())->SetTexture("Logo.png");
    scene.Add(std::move(pGameObject));


    // Title
    pGameObject = std::make_unique<DAE::GameObject>(glm::vec2{ 292, 20 });
    auto const& pFont{ DAE::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36) };
    pGameObject->AddComponent<DAE::Components::TextComponent>(
        *pGameObject.get(),
        "Programming 4 Assignment",
        pFont
        );
    scene.Add(std::move(pGameObject));

    // FPS
    pGameObject = std::make_unique<DAE::GameObject>(glm::vec2{ 10, 10 });
    pGameObject->AddComponent<DAE::Components::FPSComponent>(*pGameObject.get(), pFont);
    scene.Add(std::move(pGameObject));

    // Random circles
    pGameObject = std::make_unique<DAE::GameObject>(glm::vec2{ 200, 300 });
    DAE::GameObject& pParent{ *pGameObject.get() };
    scene.Add(std::move(pGameObject));

    pGameObject = std::make_unique<DAE::GameObject>(glm::vec2{0.f, 50.f});
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject.get())->SetTexture("RandomCircle.png");
    pGameObject->AddComponent<DAE::Components::OrbitComponent>(*pGameObject.get(), 0.5f * glm::pi<float>());
    pGameObject->SetParent(&pParent, false);
    DAE::GameObject& pParent2{ *pGameObject.get() };
    scene.Add(std::move(pGameObject));

    pGameObject = std::make_unique<DAE::GameObject>(glm::vec2{0.f, 100.f});
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject.get())->SetTexture("RandomCircle.png");
    pGameObject->AddComponent<DAE::Components::OrbitComponent>(*pGameObject.get(), -glm::pi<float>());
    pGameObject->SetParent(&pParent2, false);

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

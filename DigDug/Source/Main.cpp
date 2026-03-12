#include <iostream>

#include "Components/CacheThrashingComponent.h"
#include "Components/MovementComponent.h"
#include "Input/InputManager.h"
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

    // Character
    pGameObject = std::make_unique<DAE::GameObject>(glm::vec2{ 500, 250 });
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject.get())->SetTexture("RandomCircle.png");
    DAE::Components::MovementComponent& movementComponent {
        *pGameObject->AddComponent<DAE::Components::MovementComponent>(*pGameObject.get(), 1000.f)
    };
    scene.Add(std::move(pGameObject));

    // Input bindings
    DAE::Input::InputManager& inputManager{ DAE::Input::InputManager::GetInstance() };
    //// Movement
    inputManager.Bind({SDL_SCANCODE_W, SDL_EVENT_KEY_DOWN}, std::make_unique<DAE::Input::MoveCommand>(movementComponent, glm::vec2{ 0.f, -1.f }));
    inputManager.Bind({SDL_SCANCODE_A, SDL_EVENT_KEY_DOWN}, std::make_unique<DAE::Input::MoveCommand>(movementComponent, glm::vec2{ -1.f, 0.f }));
    inputManager.Bind({SDL_SCANCODE_S, SDL_EVENT_KEY_DOWN}, std::make_unique<DAE::Input::MoveCommand>(movementComponent, glm::vec2{ 0.f, 1.f }));
    inputManager.Bind({SDL_SCANCODE_D, SDL_EVENT_KEY_DOWN}, std::make_unique<DAE::Input::MoveCommand>(movementComponent, glm::vec2{ 1.f, 0.f }));
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

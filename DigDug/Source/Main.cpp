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
    auto pGameObject{ scene.CreateGameObject(glm::vec2{}) };
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject)->SetTexture("Background.png");

    // Logo
    pGameObject = scene.CreateGameObject(glm::vec2{358, 180});
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject)->SetTexture("Logo.png");

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
    pGameObject->AddComponent<DAE::Components::RenderComponent>(*pGameObject)->SetTexture("RandomCircle.png");
    DAE::Components::MovementComponent& movementComponent {
        *pGameObject->AddComponent<DAE::Components::MovementComponent>(*pGameObject, 500.f)
    };

    // Input bindings
    DAE::Input::InputManager& inputManager{ DAE::Input::InputManager::GetInstance() };
    //// Movement
    ///// Keyboard
    inputManager.Bind({SDL_SCANCODE_W, DAE::Input::InputType::pressed}, std::make_unique<DAE::Input::MoveCommand>(movementComponent, glm::vec2{ 0.f, -1.f }));
    inputManager.Bind({SDL_SCANCODE_A, DAE::Input::InputType::pressed}, std::make_unique<DAE::Input::MoveCommand>(movementComponent, glm::vec2{ -1.f, 0.f }));
    inputManager.Bind({SDL_SCANCODE_S, DAE::Input::InputType::pressed}, std::make_unique<DAE::Input::MoveCommand>(movementComponent, glm::vec2{ 0.f, 1.f }));
    inputManager.Bind({SDL_SCANCODE_D, DAE::Input::InputType::pressed}, std::make_unique<DAE::Input::MoveCommand>(movementComponent, glm::vec2{ 1.f, 0.f }));
    ///// Gamepad

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

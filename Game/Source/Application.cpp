// Game
#include "Constants.hpp"
#include "Application.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/LivesComponent.hpp"
#include "Components/LivesDisplayComponent.hpp"
#include "Components/PointDisplayComponent.hpp"
// Engine
#include "Engine/Scene/Scene.hpp"
#include "Engine/Components/MovementComponent.hpp"
#include "Engine/Components/AnimationComponent.hpp"
#include "Engine/Sound/SoundServiceLocator.hpp"
#include "Engine/Rendering/Font.hpp"
#include "Engine/Rendering/Sprite.hpp"
// Standard
#include <filesystem>

namespace fs = std::filesystem;

[[nodiscard]] std::string GetResourceFolderPath()
{
#if __EMSCRIPTEN__
    fs::path resourceFolderLocation = "";
#else
    auto resourceFolderName{"Resources"};
    fs::path resourceFolderLocation = std::format("./{}/", resourceFolderName);
    if (!fs::exists(resourceFolderLocation))
        resourceFolderLocation = std::format("../{}/", resourceFolderName);
#endif

    return resourceFolderLocation.generic_string();
}

Game::Application::Application()
    : Engine::Application(windowData, "Dig Dug")
{
    // cd into the resource directory
    fs::current_path(GetResourceFolderPath());

    // Playing sound
    auto& soundService{ Engine::SoundServiceLocator::GetSoundService() };
    auto const soundId{ soundService.LoadSound("GameStart.mp3") };
    soundService.SetVolume(0.05f);
    soundService.PlaySound(soundId);

    // Loading assets
    m_pFont = std::make_unique<Engine::Font>("Lingua.otf", 36);
    m_pSpriteSheet = std::make_unique<Engine::Sprite>("Sprites/DigDugSpriteSheet.png");
    m_pBackgroundTexture = std::make_unique<Engine::Sprite>("Sprites/DigDugBackground.png");

    // Background
    {
        auto& background{scene.CreateGameObject({})};
        background.AddComponent<Engine::RenderComponent>(Engine::Sprite::View{m_pBackgroundTexture.get()}, Engine::Renderer::Layer::background);
    }

    // Grid
    m_grid = std::make_unique<Grid>(tileSideLength, windowData.logicalDims);

    // Character
    {
        auto& character{scene.CreateGameObject({1.f, tileSideLength + 1})};

        // Movement component
        character.AddComponent<Engine::MovementComponent>(
            Engine::MovementComponent::Dependencies{windowData, tileSideLength},
            tileSideLength,
            75.f
        );

        // Animation component
        character.AddComponent<Engine::AnimationComponent>(Engine::AnimationComponent::Data{
            .firstSpriteView = Engine::Sprite::View{m_pSpriteSheet.get(),
                SDL_FRect{0.f, 0.f,
                static_cast<float>(tileSideLength),
                static_cast<float>(tileSideLength)}
                },
            .frameCount = 2,
            .secPerFrame = 0.1f
        });

        // Player component(must be added after animation component)
        auto& playerComponent{character.AddComponent<PlayerComponent>(PlayerComponent::Dependencies{*m_grid})};

        // Render component
        auto& characterRenderComponent{character.AddComponent<Engine::RenderComponent>(
            Engine::Sprite::View{m_pSpriteSheet.get()}
        )};
        characterRenderComponent.SetSpriteView({m_pSpriteSheet.get(), SDL_FRect{0.f, 0.f,
            static_cast<float>(tileSideLength), static_cast<float>(tileSideLength)}});

        // Lives component
        auto& livesComponent{character.AddComponent<LivesComponent>(2)};
        livesComponent.subject.BindObserver(playerComponent);

        // Lives display
        {
            auto& livesDisplay{scene.CreateGameObject(glm::vec2{10.f, static_cast<float>(windowData.dims.y) - 10.f})};
            livesDisplay.AddComponent<Engine::TextComponent>(" ", m_pFont.get()); // NOTE: Text must not be empty
            auto& livesDisplayComponent{livesDisplay.AddComponent<LivesDisplayComponent>(livesComponent)};
            livesComponent.subject.BindObserver(livesDisplayComponent);
        }

        // Point display
        {
            auto& pointDisplay{scene.CreateGameObject(glm::vec2{10.f, static_cast<float>(windowData.dims.y) - 20.f})};
            pointDisplay.AddComponent<Engine::TextComponent>("Points ", m_pFont.get());
            auto& pointDisplayComponent{pointDisplay.AddComponent<PointDisplayComponent>(playerComponent)};
            playerComponent.subject.BindObserver(pointDisplayComponent);
        }
    }
}

void Game::Application::Update()
{
    scene.Update();
}

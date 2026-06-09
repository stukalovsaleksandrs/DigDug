// Game
#include "Utils.hpp"
#include "Application.hpp"
#include "Components/PlayerComponent.hpp"
#include "Components/LivesComponent.hpp"
#include "Components/LivesDisplayComponent.hpp"
#include "Components/PointDisplayComponent.hpp"
#include "Components/AIComponent.hpp"
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
    soundService.SetVolume(0.05f);
    LoadSound(SoundName::GameStart, "GameStart.mp3");
    PlaySound(SoundName::GameStart);

    // Loading assets
    m_pFont = std::make_unique<Engine::Font>("Lingua.otf", 36);
    m_pTaizoHoriSprite = std::make_unique<Engine::Sprite>("Sprites/TaizoHori/Default.png");
    m_pGroundSprite = std::make_unique<Engine::Sprite>("Sprites/Background/Ground.png");
    m_pSkySprite = std::make_unique<Engine::Sprite>("Sprites/Background/Sky.png");

    // Ground
    {
        auto& ground{scene.CreateGameObject({})};
        ground.AddComponent<Engine::RenderComponent>(Engine::Sprite::View{m_pGroundSprite.get()}, Engine::Renderer::Layer::background);
    }

    // Levels
    // Don't move to the header, has to be added to rendering after the ground
    m_pLevelManager = std::make_unique<LevelManager>(std::vector<std::string_view>{"Levels/Level1.txt"});

    // Sky
    {
        auto& sky{scene.CreateGameObject({})};
        sky.AddComponent<Engine::RenderComponent>(Engine::Sprite::View{m_pSkySprite.get()}, Engine::Renderer::Layer::middleground);
    }

    // Character
    {
        auto& character{scene.CreateGameObject({1.f, tileSideLength + 1})};

        // Movement component
        character.AddComponent<Engine::MovementComponent>(
            Engine::MovementComponent::Dependencies{windowData, tileSideLength},
            tileSideLength,
            45.f
        );

        // Animation component
        character.AddComponent<Engine::AnimationComponent>(Engine::AnimationComponent::Data{
            .firstSpriteView = Engine::Sprite::View{m_pTaizoHoriSprite.get(),
                SDL_FRect{0.f, 0.f,
                static_cast<float>(tileSideLength),
                static_cast<float>(tileSideLength)}
                },
            .frameCount = 2,
            .secPerFrame = 0.2f
        });

        // Player component(must be added after animation component)
        auto& playerComponent{character.AddComponent<PlayerComponent>(PlayerComponent::Dependencies{*m_pLevelManager})};

        // Render component
        auto& characterRenderComponent{character.AddComponent<Engine::RenderComponent>(
            Engine::Sprite::View{m_pTaizoHoriSprite.get()}
        )};
        characterRenderComponent.SetSpriteView({m_pTaizoHoriSprite.get(), SDL_FRect{0.f, 0.f,
            static_cast<float>(tileSideLength), static_cast<float>(tileSideLength)}});

        // Lives component
        auto& livesComponent{character.AddComponent<LivesComponent>(2)};
        livesComponent.subject.BindObserver(playerComponent);

        // Lives display
        {
            auto& livesDisplay{scene.CreateGameObject(glm::vec2{10.f, static_cast<float>(windowData.physicalDims.y) - 10.f})};
            livesDisplay.AddComponent<Engine::TextComponent>(" ", m_pFont.get()); // NOTE: Text must not be empty
            auto& livesDisplayComponent{livesDisplay.AddComponent<LivesDisplayComponent>(livesComponent)};
            livesComponent.subject.BindObserver(livesDisplayComponent);
        }

        // Point display
        {
            auto& pointDisplay{scene.CreateGameObject(glm::vec2{10.f, static_cast<float>(windowData.physicalDims.y) - 20.f})};
            pointDisplay.AddComponent<Engine::TextComponent>("Points ", m_pFont.get());
            auto& pointDisplayComponent{pointDisplay.AddComponent<PointDisplayComponent>(playerComponent)};
            playerComponent.subject.BindObserver(pointDisplayComponent);
        }
    }

    // Pooka
    {
        auto& pooka{scene.CreateGameObject({windowData.logicalDims.x / 2, tileSideLength + 1})};

        // Movement component
        pooka.AddComponent<Engine::MovementComponent>(
            Engine::MovementComponent::Dependencies{windowData, tileSideLength},
            tileSideLength,
            55.f
        );

        m_pPookaSprite = std::make_unique<Engine::Sprite>("Sprites/Pooka/Default.png");

        // Animation component
        pooka.AddComponent<Engine::AnimationComponent>(Engine::AnimationComponent::Data{
            .firstSpriteView = Engine::Sprite::View{m_pPookaSprite.get(),
                SDL_FRect{0.f, 0.f,
                static_cast<float>(tileSideLength),
                static_cast<float>(tileSideLength)}
                },
            .frameCount = 2,
            .secPerFrame = 0.3f
        });

        // Render component
        auto& renderComponent{pooka.AddComponent<Engine::RenderComponent>(
            Engine::Sprite::View{m_pPookaSprite.get()}
        )};
        renderComponent.SetSpriteView({m_pPookaSprite.get(), SDL_FRect{0.f, 0.f,
            static_cast<float>(tileSideLength), static_cast<float>(tileSideLength)}});

        // AI component
        auto& aiComponent{ pooka.AddComponent<AIComponent>(PlayerComponent::Dependencies{*m_pLevelManager}) };
    }
}

void Game::Application::Update()
{
    scene.Update();
}

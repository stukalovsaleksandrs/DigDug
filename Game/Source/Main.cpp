#include "Components/LivesComponent.h"
#include "Components/LivesDisplayComponent.h"
#include "Components/PlayerComponent.h"
#include "Components/PointDisplayComponent.h"
#include "Engine/Components/MovementComponent.h"
#include "SDL3/SDL_main.h"// Required for the windows build not to give errors
#if _DEBUG && __has_include(<vld.h>)
#include <vld.h>
#endif
#include "Engine/Core/Application.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Core/ResourceManager.h"
#include "Engine/Core/Observer.h"
#include "Engine/Scene/Scene.h"
#include <filesystem>
#include <glm/glm.hpp>
namespace fs = std::filesystem;

#ifdef USE_STEAMWORKS
#ifdef WINDOWS
#pragma warning (push)
#pragma warning (disable: 4996)
#endif
#include <steam_api.h>
#if WINDOWS
#pragma warning (pop)
#endif

// Defining our achievements
enum EAchievements
{
    ACH_WIN_ONE_GAME = 0,
    ACH_WIN_100_GAMES = 1,
    ACH_TRAVEL_FAR_ACCUM = 2,
    ACH_TRAVEL_FAR_SINGLE = 3,
};

// Achievement array which will hold data about the achievements and their state
inline Engine::Achievement_t g_Achievements[] =
{
    _ACH_ID( ACH_WIN_ONE_GAME, "Winner" ),
    _ACH_ID( ACH_WIN_100_GAMES, "Champion" ),
    _ACH_ID( ACH_TRAVEL_FAR_ACCUM, "Interstellar" ),
    _ACH_ID( ACH_TRAVEL_FAR_SINGLE, "Orbiter" ),
};

// Global access to Achievements object
Engine::CSteamAchievements*	g_SteamAchievements = NULL;
#endif

float constexpr resolutionScale{ 3 };
glm::vec2 constexpr originalGameResolution{ 224.f, 288.f },
    windowResolution{ originalGameResolution * resolutionScale };

static void Load()
{
    auto& scene{ Engine::SceneManager::GetInstance().CreateScene() };

    // Character
    auto& character{ scene.CreateGameObject(glm::vec2{ 500.f, 250.f }) };

    auto& characterRenderComponent{ character.AddComponent<Engine::RenderComponent>() };
    characterRenderComponent.SetTexture("DigDugCharacter.png");

    character.AddComponent<Engine::MovementComponent>(500.f);
    auto& playerComponent{ character.AddComponent<Game::PlayerComponent>()};

    auto& livesComponent{ character.AddComponent<Game::LivesComponent>(2) };
    livesComponent.subject.BindObserver(playerComponent);

    // Lives display
    auto& livesDisplay{ scene.CreateGameObject(glm::vec2{10.f, windowResolution.y - 50.f}) };
    auto const& pFont{ Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36)};
    livesDisplay.AddComponent<Engine::TextComponent>(" ", pFont);// NOTE: Text must not be empty
    auto& livesDisplayComponent{ livesDisplay.AddComponent<Game::LivesDisplayComponent>(livesComponent) };
    livesComponent.subject.BindObserver(livesDisplayComponent);

    // Tutorial
    auto& tutorial{ scene.CreateGameObject(glm::vec2{10.f, 0.1f * windowResolution.y }) };
    auto const& pTutorialFont{ Engine::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20)};
    tutorial.AddComponent<Engine::TextComponent>("Use WASD to move Dig Dug, K to inflict damage, P to add points", pTutorialFont);

    // Point display
    auto& pointDisplay{ scene.CreateGameObject(glm::vec2{10.f, windowResolution.y - 100.f}) };
    pointDisplay.AddComponent<Engine::TextComponent>("Points ", pFont);
    auto& pointDisplayComponent{ pointDisplay.AddComponent<Game::PointDisplayComponent>(playerComponent) };
    playerComponent.subject.BindObserver(pointDisplayComponent);
}

int main(int, char*[]) {
#if USE_STEAMWORKS
    if (!SteamAPI_Init())
        throw std::runtime_error(std::string("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)."));

    g_SteamAchievements = new Engine::CSteamAchievements(g_Achievements, 4);
    // // SteamFriends()->ActivateGameOverlay("achievements");
    // std::string const name{ "ARCH_WIN_ONE_GAME" };
    // SteamUserStats()->SetAchievement(name.c_str());
    // SteamUserStats()->StoreStats();
#endif
#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    auto resourceFolderName{ "Resources" };
    fs::path data_location = std::format("./{}/", resourceFolderName);
    if(!fs::exists(data_location))
        data_location = std::format("../{}/", resourceFolderName);
#endif
    Engine::Application game(data_location, windowResolution);
    game.Run(Load);
#if USE_STEAMWORKS
    SteamAPI_Shutdown();
    if (g_SteamAchievements)
        delete g_SteamAchievements;
#endif
    return 0;
}

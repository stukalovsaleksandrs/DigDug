#ifndef ENGINE_STEAMWORKS
#define ENGINE_STEAMWORKS

namespace Engine
{
    inline void InitializeSteamworks()
    {
#if USE_STEAMWORKS
        if (!SteamAPI_Init())
            throw std::runtime_error(std::string("Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)."));

        g_SteamAchievements = new Engine::CSteamAchievements(g_Achievements, 4);
        // // SteamFriends()->ActivateGameOverlay("achievements");
        // std::string const name{ "ARCH_WIN_ONE_GAME" };
        // SteamUserStats()->SetAchievement(name.c_str());
        // SteamUserStats()->StoreStats();
#endif
    }

    inline void ShutdownSteamWorks()
    {
#if USE_STEAMWORKS
        SteamAPI_Shutdown();
        if (g_SteamAchievements)
            delete g_SteamAchievements;
#endif
    }

}

#endif// ENGINE_STEAMWORKS

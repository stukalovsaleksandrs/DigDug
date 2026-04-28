#ifndef GAME_ACHIEVEMENTS_H
#define GAME_ACHIEVEMENTS_H

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
#endif// GAME_ACHIEVEMENTS_H

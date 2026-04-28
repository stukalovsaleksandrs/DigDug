#ifndef SE_ACHIEVEMENTS_H
#define SE_ACHIEVEMENTS_H
#ifdef USE_STEAMWORKS

#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }
#include <isteamuserstats.h>

#include "Engine/Core/Observer.h"

namespace Engine
{
    struct Achievement_t
    {
        int m_eAchievementID;
        const char *m_pchAchievementID;
        char m_rgchName[128];
        char m_rgchDescription[256];
        bool m_bAchieved;
        int m_iIconImage;
    };

    class CSteamAchievements : Observer
    {
    public:
        CSteamAchievements(Achievement_t *Achievements, int NumAchievements);

        void OnNotify(Event const event, Subject const&) noexcept;

        bool Initialize();
        bool SetAchievement(const char* ID);

        STEAM_CALLBACK( CSteamAchievements, OnUserStatsStored, UserStatsStored_t,
            m_CallbackUserStatsStored );
        STEAM_CALLBACK(CSteamAchievements, OnAchievementStored,
            UserAchievementStored_t, m_CallbackAchievementStored );

    private:
        uint64 m_iAppID; // Our current AppID
        Achievement_t *m_pAchievements; // Achievements data
        int m_iNumAchievements; // The number of Achievements
        bool m_bInitialized; // Are we ready to use the API?

    };
}

#endif
#endif// SE_ACHIEVEMENTS_H

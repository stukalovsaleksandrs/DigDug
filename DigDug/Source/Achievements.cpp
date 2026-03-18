#ifdef USE_STEAMWORKS
#include "Achievements.h"

#include <isteamuser.h>
#include <isteamutils.h>
#include <print>

DAE::CSteamAchievements::CSteamAchievements(Achievement_t *Achievements, int const NumAchievements):
 m_CallbackUserStatsStored( this, &CSteamAchievements::OnUserStatsStored ),
 m_CallbackAchievementStored( this, &CSteamAchievements::OnAchievementStored ),
 m_iAppID( 0 ),
 m_bInitialized( false )
{
    m_iAppID = SteamUtils()->GetAppID();
    m_pAchievements = Achievements;
    m_iNumAchievements = NumAchievements;
    m_bInitialized = Initialize();
}

void DAE::CSteamAchievements::OnNotify(Event const event, Subject const&) noexcept
{
    switch (event.id)
    {
        case MakeSDBMHash("OnCollected5Points"):
        {

            break;
        }
    default: ;
    }
}

bool DAE::CSteamAchievements::Initialize()
{
    // Is Steam loaded? If not we can't get stats.
    if ( NULL == SteamUserStats() || NULL == SteamUser() )
    {
        return false;
    }
    // Is the user logged on?  If not we can't access stats.
    if ( !SteamUser()->BLoggedOn() )
    {
        return false;
    }

    return true;
}

bool DAE::CSteamAchievements::SetAchievement(const char* ID)
{
    // Have we received a call back from Steam yet?
    if (m_bInitialized)
    {
        SteamUserStats()->SetAchievement(ID);
        return SteamUserStats()->StoreStats();
    }
    // If not then we can't set achievements yet
    return false;
}

void DAE::CSteamAchievements::OnUserStatsStored(UserStatsStored_t* pParam)
{
    // we may get callbacks for other games' stats arriving, ignore them
    if ( m_iAppID == pParam->m_nGameID )
    {
        if ( k_EResultOK == pParam->m_eResult )
        {
            std::println( "Stored stats for Steam\n" );
        }
        else
        {
            std::println("StatsStored failed");
        }
    }
}

void DAE::CSteamAchievements::OnAchievementStored( UserAchievementStored_t *pCallback )
{
    // we may get callbacks for other games' stats arriving, ignore them
    if ( m_iAppID == pCallback->m_nGameID )
    {
        std::println( "Stored Achievement for Steam\n" );
    }
}
#endif


#ifndef __AGSWORKS_SERVICE__AGSWORKSDRIVER_H
#define __AGSWORKS_SERVICE__AGSWORKSDRIVER_H

#include <memory>

namespace AGSWorks
{

enum class ServiceType {
    eNone = -1,
    eDummy = 0,
    eSteam = 1,
    eGog = 2
};

class AGSWorksDriver
{
public:
    AGSWorksDriver() = default;
    virtual ~AGSWorksDriver() = default;

    virtual void Shutdown();
    virtual ServiceType GetServiceType();
    virtual void Init();

    virtual void AgsWorksCompat_Initialize(const char *clientID, const char *clientSecret);
    virtual bool AgsWorksCompat_IsAchievementAchieved(const char* achievment_name);
    virtual bool AgsWorksCompat_SetAchievementAchieved(const char* achievment_name);
    virtual bool AgsWorksCompat_ResetAchievement(const char* achievment_name);
    virtual int AgsWorksCompat_GetIntStat(const char* stat_name);
    virtual float AgsWorksCompat_GetFloatStat(const char* stat_name);
    virtual float AgsWorksCompat_GetAverageRateStat(const char* stat_name);
    virtual bool AgsWorksCompat_SetIntStat(const char* stat_name, int value);
    virtual bool AgsWorksCompat_SetFloatStat(const char* stat_name, float value);
    virtual bool AgsWorksCompat_UpdateAverageRateStat(const char* stat_name, float numerator, float denominator);
    virtual void AgsWorksCompat_ResetAchievements();
    virtual void AgsWorksCompat_ResetStats();
    virtual void AgsWorksCompat_ResetStatsAndAchievements();
    virtual bool AgsWorksCompat_IsInitialized();
    virtual const char* AgsWorksCompat_GetCurrentLeaderboardName();
    virtual void AgsWorksCompat_RequestLeaderboard(const char *leaderboardName, int type, int limit);
    virtual bool AgsWorksCompat_UploadScore(int score);
    virtual const char* AgsWorksCompat_GetLeaderNames(int index);
    virtual int AgsWorksCompat_GetLeaderScores(int index);
    virtual int AgsWorksCompat_GetLeaderCount();
    virtual const char * AgsWorksCompat_GetUserName();
    virtual const char * AgsWorksCompat_GetCurrentGameLanguage();
    virtual void AgsWorksCompat_FindLeaderboard(const char *leaderboardName);
};

} // namespace AGSWorks
#endif // __AGSWORKS_SERVICE__AGSWORKSDRIVER_H
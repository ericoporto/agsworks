#ifndef __AGSWORKS_SERVICE__STEAM_H
#define __AGSWORKS_SERVICE__STEAM_H

#include "service/base/agsworksdriver.h"
#include "dynapi_steam.h"

namespace AGSWorks
{

struct SteamWorksDriver : AGSWorksDriver {
private:
    DynApiSteam _s = DynApiSteam();
    bool _initialized = false;
    ISteamClient* _steamClient = nullptr;
    ISteamUserStats* _steamUserStats = nullptr;

public:
    static SteamWorksDriver* CreateDriver();
    ServiceType GetServiceType() override;
    void Init() override;
    void Shutdown() override;
    bool AgsWorksCompat_IsInitialized() override;
    bool AgsWorksCompat_IsAchievementAchieved(const char* achievment_name) override;
    bool AgsWorksCompat_SetAchievementAchieved(const char* achievment_name) override;
    bool AgsWorksCompat_ResetAchievement(const char* achievment_name) override;
    void AgsWorksCompat_ResetAchievements() override;
    void AgsWorksCompat_ResetStats() override;
    void AgsWorksCompat_ResetStatsAndAchievements() override;
};

} // namespace AGSWorks

#endif // __AGSWORKS_SERVICE__STEAM_H
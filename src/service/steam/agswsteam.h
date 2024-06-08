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
};

} // namespace AGSWorks

#endif // __AGSWORKS_SERVICE__STEAM_H
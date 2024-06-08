#ifndef __AGSWORKS_SERVICE__STEAM_H
#define __AGSWORKS_SERVICE__STEAM_H

#include "service/base/agsworksdriver.h"
#include "dynapi_steam.h"

namespace AGSWorks
{

struct SteamWorksDriver : AGSWorksDriver {
private:
    DynApiSteam _s;
    bool _initialized;
    ISteamClient* _steamClient;
    ISteamUserStats* _steamUserStats;

public:
    static SteamWorksDriver* CreateDriver();
    ServiceType GetServiceType() override;
    void Init() override;
    void Shutdown() override;
};

} // namespace AGSWorks

#endif // __AGSWORKS_SERVICE__STEAM_H
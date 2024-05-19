#ifndef __AGSWORKS_SERVICE__STEAM_H
#define __AGSWORKS_SERVICE__STEAM_H

#include "service/base/agsworksdriver.h"

namespace AGSWorks
{

struct SteamWorksDriver : AGSWorksDriver {
    static SteamWorksDriver* CreateDriver();
    ServiceType GetServiceType() override;
    void Init() override;
};

} // namespace AGSWorks

#endif // __AGSWORKS_SERVICE__STEAM_H
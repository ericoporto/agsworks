#ifndef __AGSWORKS_SERVICE__GOG_H
#define __AGSWORKS_SERVICE__GOG_H

#include "service/base/agsworksdriver.h"

namespace AGSWorks
{

struct GogWorksDriver : AGSWorksDriver {
    static GogWorksDriver* CreateDriver();
    ServiceType GetServiceType() override;

    void AgsWorksCompat_Initialize(const char *clientID, const char *clientSecret) override;
};

} // namespace AGSWorks

#endif // __AGSWORKS_SERVICE__GOG_H
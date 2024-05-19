
#ifndef __AGSWORKS_SERVICE__DUMMY_H
#define __AGSWORKS_SERVICE__DUMMY_H

#include "agsworksdriver.h"


namespace AGSWorks
{

struct DummyWorksDriver : AGSWorksDriver {
    static DummyWorksDriver* CreateDriver();
    ServiceType GetServiceType() override;
};

} // namespace AGSWorks

#endif // __AGSWORKS_SERVICE__DUMMY_H
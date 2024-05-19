
#include "agswdummy.h"

namespace AGSWorks
{

DummyWorksDriver* DummyWorksDriver::CreateDriver()
{
    return new DummyWorksDriver();
}

ServiceType DummyWorksDriver::GetServiceType()
{
    return ServiceType::eDummy;
}

} // namespace AGSWorks

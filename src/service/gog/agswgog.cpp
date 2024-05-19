
#include "agswgog.h"

namespace AGSWorks
{

GogWorksDriver* GogWorksDriver::CreateDriver()
{
    return new GogWorksDriver();
}

ServiceType GogWorksDriver::GetServiceType()
{
    return ServiceType::eGog;
}

void GogWorksDriver::AgsWorksCompat_Initialize(const char *clientID, const char *clientSecret) {
    // do nothing
}

} // namespace AGSWorks

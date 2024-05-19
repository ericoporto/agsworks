
#include "agswsteam.h"

namespace AGSWorks
{

void SteamWorksDriver::Init()
{
  // tbd
}


SteamWorksDriver* SteamWorksDriver::CreateDriver()
{
    return new SteamWorksDriver();
}

ServiceType SteamWorksDriver::GetServiceType()
{
    return ServiceType::eSteam;
}

} // namespace AGSWorks

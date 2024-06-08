
#include <algorithm>
#include <string>
#include "core/platform.h"
#include "misc/utils.h"
#include "dynamicserviceloader.h"
#include "service/base/agswdummy.h"
#include "service/gog/agswgog.h"
#include "service/steam/agswsteam.h"


namespace AGSWorks
{

void DynamicServiceLoader::Init(std::string this_dll_dir) {
    auto lookup = std::vector<std::string>();
    lookup.emplace_back(".");
    lookup.emplace_back(GetCwd());
    lookup.emplace_back(this_dll_dir);

#if AGS_PLATFORM_OS_WINDOWS
#if AGS_PLATFORM_64BIT
    const std::string steam_libname = "steam_api64";
    const std::string gog_libname = "Galaxy64";
#else
    const std::string steam_libname = "steam_api";
    const std::string gog_libname = "Galaxy";
#endif
#elif AGS_PLATFORM_OS_MACOS
    const std::string steam_libname = "steam_api";
    const std::string gog_libname = "Galaxy";
#else
    const std::string steam_libname = "steam_api";
    const std::string gog_libname = "Galaxy";
#endif

    // check if Steam is here
    _lib.Load(steam_libname, lookup);
    if(_lib.IsLoaded()){
        // it's steam
        _type = ServiceType::eSteam;
        return;
    }

    // check if Gog is here
    _lib.Load(gog_libname, lookup);
    if(_lib.IsLoaded()){
        // it's Gog
        _type = ServiceType::eGog;
        return;
    }

    _type = ServiceType::eDummy;
}

void DynamicServiceLoader::Shutdown() {
    // tbd
}

AGSWorksDriver* DynamicServiceLoader::CreateWorksDriver() {
    ServiceType type = this->_type;
    switch (type) {
        case ServiceType::eDummy:
            return DummyWorksDriver::CreateDriver();
            break;
        case ServiceType::eSteam:
            return SteamWorksDriver::CreateDriver();
            break;
        case ServiceType::eGog:
            return GogWorksDriver::CreateDriver();
            break;
        case ServiceType::eNone:
            return nullptr;
            break;
    }
    return nullptr;
}

} // namespace AGSWorks
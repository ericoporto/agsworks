
#ifndef AGSWORKS_DYNAMICSERVICELOADER_H
#define AGSWORKS_DYNAMICSERVICELOADER_H

#include "misc/library.h"
#include "service/base/agsworksdriver.h"

namespace AGSWorks
{

class DynamicServiceLoader
{

private:
    DynamicServiceLoader() = default;
    Library _lib = Library();
    ServiceType _type = ServiceType::eDummy;

    void InitImpl(std::string &this_dll_dir);
    AGSWorksDriver* CreateWorksDriverImpl();
    void ShutdownImpl();
    void* GetFunctionAddressImpl(const std::string &fn_name);

public:
    static DynamicServiceLoader& get(){
        static DynamicServiceLoader instance;
        return instance;
    }
    static ServiceType GetServiceType();
    static void* GetFunctionAddress(const std::string &fn_name);
    static void Init(std::string &this_dll_dir);
    static AGSWorksDriver* CreateWorksDriver();
    static void Shutdown();
};

} // namespace AGSWorks

#endif // AGSWORKS_DYNAMICSERVICELOADER_H

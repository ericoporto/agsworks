
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

public:
    static DynamicServiceLoader& get(){
        static DynamicServiceLoader instance;
        return instance;
    }
    inline static ServiceType GetServiceType()
    {
        return get()._type;
    }
    inline static void* GetFunctionAddress(std::string &fn_name)
    {
        if(!(get()._lib.IsLoaded()))
            return nullptr;

        return get()._lib.GetFunctionAddress(fn_name);
    }

    void Init(std::string this_dll_dir);
    AGSWorksDriver* CreatetWorksDriver();
    void Shutdown();
};

} // namespace AGSWorks

#endif // AGSWORKS_DYNAMICSERVICELOADER_H
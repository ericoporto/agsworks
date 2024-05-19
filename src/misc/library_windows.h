//
// Created by erico on 5/18/2024.
//

#ifndef AGSWORKS_MISC_LIBRARY_WINDOWS_H
#define AGSWORKS_MISC_LIBRARY_WINDOWS_H


#include "base_library.h"
#include "core/platform.h"
#if AGS_PLATFORM_OS_WINDOWS

namespace AGSWorks
{

class WindowsLibrary final : public BaseLibrary
{
public:
    WindowsLibrary() = default;
    WindowsLibrary(const WindowsLibrary&) = delete;
    WindowsLibrary(WindowsLibrary &&other);
    ~WindowsLibrary() override;
    std::string GetFilenameForLib(std::string libname) override;

    bool Load(std::string libname, std::vector<std::string> lookup) override;

    void Unload() override;

    bool IsLoaded() const override;
    void *GetFunctionAddress(const std::string &fn_name) override;

private:
    void * TryLoad(const std::string &path);

    void * TryLoadAnywhere(const std::string &libfile, const std::vector<std::string> &lookup, std::string &path);

    void * _library = nullptr;
};

typedef WindowsLibrary Library;

} // namespace AGSWorks

#endif // AGS_PLATFORM_OS_WINDOWS

#endif //AGSWORKS_MISC_LIBRARY_WINDOWS_H

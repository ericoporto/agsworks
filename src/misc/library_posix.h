//
// Created by erico on 5/18/2024.
//

#ifndef AGSWORKS_MISC_LIBRARY_POSIX_H
#define AGSWORKS_MISC_LIBRARY_POSIX_H

#include "base_library.h"
#include "core/platform.h"

#if AGS_PLATFORM_OS_LINUX \
   || AGS_PLATFORM_OS_MACOS \
   || AGS_PLATFORM_OS_ANDROID \
   || AGS_PLATFORM_OS_FREEBSD


namespace AGSWorks
{

class PosixLibrary final : public BaseLibrary
{
public:
    PosixLibrary() = default;
    PosixLibrary(const PosixLibrary&) = delete;
    PosixLibrary(PosixLibrary &&other);
    ~PosixLibrary() override;

    std::string GetFilenameForLib(const std::string libname) override;

    bool Load(const std::string libname, const std::vector<std::string> lookup) override;

    void Unload() override;

    bool IsLoaded() const override;

    void *GetFunctionAddress(const std::string &fn_name) override;

private:
    void *TryLoad(const std::string &path);

    void *TryLoadAnywhere(const std::string &libfile, const std::vector<std::string> &lookup, std::string &path);

    void *_library = nullptr;
};

typedef PosixLibrary Library;

} // namespace AGSWorks

#endif

#endif //AGSWORKS_MISC_LIBRARY_POSIX_H

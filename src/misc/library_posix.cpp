//
// Created by erico on 5/18/2024.
//
#include "library_posix.h"

#if AGS_PLATFORM_OS_LINUX \
   || AGS_PLATFORM_OS_MACOS \
   || AGS_PLATFORM_OS_ANDROID \
   || AGS_PLATFORM_OS_FREEBSD

#include <dlfcn.h>
#include <utility>
#include "awlog.h"

namespace AGSWorks {

AGSWorks::PosixLibrary::PosixLibrary(PosixLibrary &&other)
{
    _library = other._library;
    other._library = NULL;
    _name = std::move(other._name);
    _filename = std::move(other._filename);
    _path = std::move(other._path);
}

AGSWorks::PosixLibrary::~PosixLibrary()
{
    Unload();
}

std::string AGSWorks::PosixLibrary::GetFilenameForLib(const std::string libname)
{
    std::string filename = "lib";
#if AGS_PLATFORM_OS_MACOS
    filename = filename + libname + ".dylib";
#else
    filename = filename + libname + ".so";
#endif
    return filename;
}

bool AGSWorks::PosixLibrary::Load(const std::string libname, const std::vector<std::string> lookup)
{

    Unload();
    std::string libfile = GetFilenameForLib(libname);
    std::string path; // save last tried path
    void *lib = TryLoadAnywhere(libfile, lookup, path);
    if (!lib)
        return false;
    _library = lib;
    _name = libname;
    // _filename = Path::GetFilename(path);
    _path = path;
    return true;
}

void AGSWorks::PosixLibrary::Unload()
{
    if (_library) {
        dlclose(_library);
        _library = nullptr;
        _name = "";
        _filename = "";
        _path = "";
    }

}

bool AGSWorks::PosixLibrary::IsLoaded() const
{
    return _library != nullptr;
}

void *AGSWorks::PosixLibrary::GetFunctionAddress(const std::string &fn_name)
{
    if (!_library)
        return nullptr;
    return dlsym(_library, fn_name.c_str());
}

void *AGSWorks::PosixLibrary::TryLoad(const std::string &path)
{
    void *lib = dlopen(path.c_str(), RTLD_LAZY);
    if (!lib) {
        std::string log_msg = "dlopen error: ";
        log_msg += dlerror();
        LOG_DEBUG(log_msg);
    }
    return lib;
}

void *
AGSWorks::PosixLibrary::TryLoadAnywhere(const std::string &libfile, const std::vector<std::string> &lookup, std::string &path)
{
    // Try rpath first
    path = libfile;
    void *lib = TryLoad(path);
    if (lib)
        return lib;

    // Try current path
    path = ConcatPaths(".", libfile);
    lib = TryLoad(path);
    if (lib)
        return lib;

    // Try lookup paths last
    for (const auto &p: lookup) {
        path = ConcatPaths(p, libfile);
        lib = TryLoad(path);
        if (lib)
            return lib;
    }
    return nullptr;
}

} // namespace AGSWorks

#endif
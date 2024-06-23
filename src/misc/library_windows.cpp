//
// Created by erico on 5/18/2024.
//
#include "core/platform.h"
#if AGS_PLATFORM_OS_WINDOWS
#include "library_windows.h"
#include "Windows.h"
#include "awlog.h"

namespace AGSWorks {

WindowsLibrary::WindowsLibrary(AGSWorks::WindowsLibrary &&other)
{
    _library = other._library;
    other._library = nullptr;
    _name = std::move(other._name);
    _filename = std::move(other._filename);
    _path = std::move(other._path);
}

WindowsLibrary::~WindowsLibrary()
{
    Unload();
}

std::string WindowsLibrary::GetFilenameForLib(std::string libname)
{
    return libname + ".dll";
}

bool WindowsLibrary::Load(std::string libname, std::vector<std::string> lookup)
{
    Unload();
    std::string libfile = GetFilenameForLib(libname);
    std::string path; // save last tried path
    HMODULE lib = static_cast<HMODULE>(TryLoadAnywhere(libfile, lookup, path));
    if (lib == nullptr)
        return false;
    _library = lib;
    _name = libname;
    _filename = libfile;
    _path = path;
    return true;
}

void WindowsLibrary::Unload()
{
    if (_library)
    {
        FreeLibrary(static_cast<HMODULE>(_library));
        _library = nullptr;
        _name = "";
        _filename = "";
        _path = "";
    }
}

bool WindowsLibrary::IsLoaded() const
{
    return _library != nullptr;
}

void *WindowsLibrary::GetFunctionAddress(const std::string &fn_name)
{
    if (!_library)
        return nullptr;
    return reinterpret_cast<void *>(GetProcAddress(static_cast<HMODULE>(_library), fn_name.c_str()));
}

void *WindowsLibrary::TryLoad(const std::string &path)
{
    std::string log_msg = "Try library path：　";
    log_msg += path;
    LOG_DEBUG(log_msg);
    WCHAR wpath[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, path.c_str(), -1, wpath, MAX_PATH);
    return LoadLibraryW(wpath);
}

void *WindowsLibrary::TryLoadAnywhere(const std::string &libfile, const std::vector<std::string> &lookup, std::string &path)
{
    // First try default system search
    path = libfile;
    HMODULE lib = static_cast<HMODULE>(TryLoad(path));
    if (lib)
        return lib;

    // Try lookup paths last
    for (const auto& p : lookup)
    {
        path = ConcatPaths(p, libfile);
        lib = static_cast<HMODULE>(TryLoad(path));
        if (lib)
            return lib;
    }

    return nullptr;
}

} // namespace AGSWorks

#endif // AGS_PLATFORM_OS_WINDOWS

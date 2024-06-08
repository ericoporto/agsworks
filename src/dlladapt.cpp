//
// Created by erico on 5/14/2024.
//

#include "core/platform.h"
#include "dlladapt.h"
#if AGS_PLATFORM_OS_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#if AGS_PLATFORM_OS_MACOS || AGS_PLATFORM_OS_IOS
#include <mach-o/dyld.h>
#endif
#if AGS_PLATFORM_OS_LINUX
#include <unistd.h>
#endif
#include <climits>  // For PATH_MAX
#include <string>
#include <algorithm>

namespace AGSWorks
{

    // this function below is modded around one from protobuf, but there's a good chance it's wrong...
#ifdef _WIN32
bool GetDllAbsolutePath(std::string& path, HMODULE hModule)
#else
bool GetDllAbsolutePath(std::string& path)
#endif
{
#ifdef _WIN32
    char buffer[MAX_PATH];
    int len = GetModuleFileNameA(hModule, buffer, MAX_PATH);
#elif defined(__APPLE__)
    char buffer[PATH_MAX];
    int len = 0;

    char dirtybuffer[PATH_MAX];
    uint32_t size = sizeof(dirtybuffer);
    if (_NSGetExecutablePath(dirtybuffer, &size) == 0) {
        realpath(dirtybuffer, buffer);
        len = strlen(buffer);
    }
    #elif defined(__FreeBSD__)
    char buffer[PATH_MAX];
    size_t len = PATH_MAX;
    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1};
    if (sysctl(mib, 4, &buffer, &len, nullptr, 0) != 0) {
        len = 0;
    }
    #else
    char buffer[PATH_MAX];
    int len = readlink("/proc/self/exe", buffer, PATH_MAX);
#endif
    if (len > 0) {
        path.assign(buffer, len);
        return true;
    } else {
        return false;
    }
}


#ifdef _WIN32
void DllAdapt::SetModule(void *hModule) {
    _hModule = hModule;
}
#endif

void DllAdapt::Init() {
#ifdef _WIN32
    if(GetDllAbsolutePath(_dllPath, (HMODULE)_hModule))
#else
    if(GetDllAbsolutePath(DllPath))
#endif
    {
        _dllName = _dllPath.substr(_dllPath.find_last_of("/\\") + 1);
        std::string name_without_extension = _dllName.substr(0, _dllName.find_last_of('.'));
#ifndef _WIN32
        // In non windows platforms the string is prefixed by a "lib"
        name_without_extension = name_without_extension.substr(3);
#endif
        _dllSimpleName = name_without_extension;
        std::transform(_dllSimpleName.begin(), _dllSimpleName.end(), _dllSimpleNameLower.begin(),
                               [](unsigned char c){ return std::tolower(c); });

        _isAgsSteam = _dllSimpleNameLower == "agsteam"||
             _dllSimpleNameLower == "agsteam-unified" ||
             _dllSimpleNameLower == "agsteam-disjoint";
        _isAgsGalaxy = _dllSimpleNameLower == "agsgalaxy"||
              _dllSimpleNameLower == "agsgalaxy-unified" ||
              _dllSimpleNameLower == "agsgalaxy-disjoint";
        _isAnyAdapted = _isAgsSteam || _isAgsGalaxy;
    }
}

std::string DllAdapt::GetDllDir() const {
    std::string dll_dir = _dllPath.substr(0,_dllPath.find_last_of("/\\") + 1);
    return dll_dir;
}

bool DllAdapt::IsAgsSteam() const {
    return _isAgsSteam;
}

bool DllAdapt::IsAgsGalaxy() const {
    return _isAgsGalaxy;
}

bool DllAdapt::IsAnyAdapted() const {
    return _isAnyAdapted;
}

} // AGSWorks
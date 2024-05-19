//
// Created by erico on 5/14/2024.
//

#include "dlladapt.h"
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
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
    HModule = hModule;
}

void DllAdapt::Init() {
#ifdef _WIN32
    if(GetDllAbsolutePath(DllPath, (HMODULE)HModule))
#else
    if(GetDllAbsolutePath(DllPath))
#endif
    {
        DllName = DllPath.substr(DllPath.find_last_of("/\\") + 1);
        std::string name_without_extension = DllName.substr(0, DllName.find_last_of('.'));
#ifndef _WIN32
        // In non windows platforms the string is prefixed by a "lib"
        name_without_extension = name_without_extension.substr(3);
#endif
        DllSimpleName = name_without_extension;
        std::transform(DllSimpleName.begin(), DllSimpleName.end(), DllSimpleNameLower.begin(),
                               [](unsigned char c){ return std::tolower(c); });

        IsAgsSteam = DllSimpleNameLower == "agsteam"||
                 DllSimpleNameLower == "agsteam-unified" ||
                 DllSimpleNameLower == "agsteam-disjoint";
        IsAgsGalaxy = DllSimpleNameLower == "agsgalaxy"||
                   DllSimpleNameLower == "agsgalaxy-unified" ||
                   DllSimpleNameLower == "agsgalaxy-disjoint";
        IsAnyAdapted = IsAgsSteam || IsAgsGalaxy;
    }
}

    std::string DllAdapt::GetDllDir() {
        std::string dll_dir = DllPath.substr(0,DllPath.find_last_of("/\\") + 1);
        return dll_dir;
    }

#endif
} // AGSWorks
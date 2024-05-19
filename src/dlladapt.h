//
// Created by erico on 5/14/2024.
//

#ifndef AGSWORKS_DLLADAPT_H
#define AGSWORKS_DLLADAPT_H

#include <string>

namespace AGSWorks {

    class DllAdapt {
        std::string DllName;
        std::string DllPath;
        std::string DllSimpleName;
        std::string DllSimpleNameLower;

#ifdef _WIN32
        void * HModule;
#endif

    public:
        void Init();
#ifdef _WIN32
        void SetModule(void * hModule);
#endif

        std::string GetDllDir();
        bool IsAgsSteam;
        bool IsAgsGalaxy;
        bool IsAnyAdapted;
    };

} // AGSWorks

#endif //AGSWORKS_DLLADAPT_H

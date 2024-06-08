//
// Created by erico on 5/14/2024.
//

#ifndef AGSWORKS_DLLADAPT_H
#define AGSWORKS_DLLADAPT_H

#include <string>

namespace AGSWorks {

    class DllAdapt {
    public:
        bool IsAgsSteam() const;
        bool IsAgsGalaxy() const;
        bool IsAnyAdapted() const;
        void Init();
#ifdef _WIN32
        void SetModule(void * hModule);
#endif
        std::string GetDllDir() const;

    private:
        bool _isAgsSteam = false;
        bool _isAgsGalaxy = false;
        bool _isAnyAdapted = false;
        std::string _dllName;
        std::string _dllPath;
        std::string _dllSimpleName;
        std::string _dllSimpleNameLower;
#ifdef _WIN32
        void * _hModule = nullptr;
#endif
    };

} // AGSWorks

#endif //AGSWORKS_DLLADAPT_H

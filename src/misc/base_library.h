//=============================================================================
//
// I am copying the code below directly from AGS until I can become a more creative person
// it follows the following license and copyright
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-2024 various contributors
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// https://opensource.org/license/artistic-2-0/
//
//=============================================================================

#ifndef AGSWORKS_MISC_BASE_LIBRARY_H
#define AGSWORKS_MISC_BASE_LIBRARY_H

#include <string>
#include <vector>
#include "utils.h"

namespace AGSWorks
{
    class BaseLibrary
    {
    public:
        BaseLibrary() = default;
        BaseLibrary(const BaseLibrary&) = delete;
        BaseLibrary(BaseLibrary&&) = default;
        virtual ~BaseLibrary() = default;

        // Get library name; returns empty string if not loaded
        inline std::string GetName() const { return _name; }
        // Get actual filename; returns empty string if not loaded
        inline std::string GetFileName() const { return _filename; }
        // Get path used to load the library; or empty string is not loaded.
        // NOTE: this is NOT a fully qualified path, but a lookup path.
        inline std::string GetPath() const { return _path; }

        // Returns expected filename form for the dynamic library of a given name
        virtual std::string GetFilenameForLib(std::string libname) = 0;

        // Try load a library of a given name, optionally looking it up in the list of paths
        virtual bool Load(std::string libname, std::vector<std::string> lookup) = 0;
        // Unload a library; does nothing if was not loaded
        virtual void Unload() = 0;
        // Tells if library is loaded
        virtual bool IsLoaded() const = 0;
        // Tries to get a function address from a loaded library
        virtual void *GetFunctionAddress(const std::string &fn_name) = 0;

    protected:
        std::string _name;
        std::string _filename;
        std::string _path;
    };

} // namespace AGSWorks

#endif //AGSWORKS_MISC_BASE_LIBRARY_H

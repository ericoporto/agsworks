//
// Created by erico on 5/18/2024.
//

#include "core/platform.h"
#include <algorithm>
#include <string>
#if AGS_PLATFORM_OS_WINDOWS
#include "Windows.h"
#include <direct.h>
#define getcwd _getcwd
#else
#include <unistd.h>
#endif

#include "utils.h"

namespace AGSWorks
{
    std::string MergeSequences(std::string str, char c) {
        bool got_one = false;
        for (auto it = str.begin(); it != str.end();) {
            if ((*it) == c) {
                if (got_one) {
                    it = str.erase(it);
                    continue;
                }
                got_one = true;
            } else {
                got_one = false;
            }
            it++;
        }
        return str;
    }

    void FixupPath(std::string &path) {
#if AGS_PLATFORM_OS_WINDOWS
        std::replace(path.begin(), path.end(), '\\', '/'); // bring Windows path separators to uniform style
#endif
        path = MergeSequences(path, '/');
    }

    std::string ConcatPaths(std::string parent, std::string child) {
        if (parent.empty())
            return child;
        if (child.empty())
            return parent;

        std::string path = parent + "/" + child;
        FixupPath(path);
        return path;
    }

    std::string GetCwd() {
#if AGS_PLATFORM_OS_WINDOWS
        char buff[MAX_PATH];
#else
        char buff[PATH_MAX];
#endif
        getcwd( buff, sizeof(buff));
        std::string cwd( buff );
        return cwd;
    }

} // namespace AGSWorks
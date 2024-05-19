//
// Created by erico on 5/19/2024.
//

#ifndef AGSWORKS_LIBRARY_H
#define AGSWORKS_LIBRARY_H

#include "base_library.h"
#include "core/platform.h"

#if AGS_PLATFORM_OS_WINDOWS
#include "library_windows.h"
#endif
#if AGS_PLATFORM_OS_LINUX \
   || AGS_PLATFORM_OS_MACOS \
   || AGS_PLATFORM_OS_ANDROID \
   || AGS_PLATFORM_OS_FREEBSD
#include "library_posix.h"
#endif

#endif //AGSWORKS_LIBRARY_H

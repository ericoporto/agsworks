cmake_minimum_required(VERSION 3.14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_SOURCE_DIR}/CMake/c_flag_overrides.cmake)
set(CMAKE_USER_MAKE_RULES_OVERRIDE_CXX ${CMAKE_CURRENT_SOURCE_DIR}/CMake/cxx_flag_overrides.cmake)

project(AGSWorks
        VERSION 0.1.0
        DESCRIPTION "AGS Plugin to connect to game stores."
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 14)

if(${CMAKE_BUILD_TYPE} MATCHES Release)
    message("CMAKE_CXX_FLAGS_RELEASE_INIT: ${CMAKE_CXX_FLAGS_RELEASE_INIT}")
elseif(${CMAKE_BUILD_TYPE} MATCHES Debug)
    message("CMAKE_CXX_FLAGS_DEBUG_INIT: ${CMAKE_CXX_FLAGS_DEBUG_INIT}")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # WIN32 is set by CMake for any Windows platform
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LINUX TRUE)
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOS TRUE)
else()
    message(FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}")
endif ()

message("----------------------------------------")
message( "CMAKE_SYSTEM_NAME:" ${CMAKE_SYSTEM_NAME} )
message( "WIN32:" ${WIN32} )
message( "LINUX:" ${LINUX} )
message( "MACOS:" ${MACOS} )
message("----------------------------------------")

if (WIN32 AND NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Windows builds only support 32bit for now")
endif()

add_compile_definitions("_FILE_OFFSET_BITS=64")

add_compile_definitions("$<$<CONFIG:DEBUG>:_DEBUG>")
add_compile_definitions("$<$<CONFIG:RELEASE>:NDEBUG>")

add_library(AGSWorks SHARED)

set_target_properties(AGSWorks PROPERTIES POSITION_INDEPENDENT_CODE ON)

set_target_properties(AGSWorks PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS NO
        C_STANDARD 11
        C_EXTENSIONS NO
)

target_include_directories(AGSWorks PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/")
target_include_directories(AGSWorks PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/")

# this is a bunch of stuff to make sure we have ags source code somewhere and we just need it's headers
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../ags/Engine/plugin/agsplugin.h")
    target_include_directories(AGSWorks PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../ags/Engine")
    target_include_directories(AGSWorks PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../ags/Common")
else ()
    include(FetchContent)
    set(FETCHCONTENT_UPDATES_DISCONNECTED on)
    # until 3.6.1.25 is released use commit for Log in plugin API
    FetchContent_Declare(
            ags_content
            DOWNLOAD_EXTRACT_TIMESTAMP TRUE
            URL https://github.com/adventuregamestudio/ags/archive/69f6960ea7ad7d9e9b3797d39f79287d9d98ce14.tar.gz
            URL_HASH SHA1=9765a08f086101f31d3416a380cbc2ede4e02090
    )
    FetchContent_GetProperties(ags_content)
    if(NOT ags_content)
        FetchContent_Populate(ags_content)
        target_include_directories(AGSWorks PUBLIC ${ags_content_SOURCE_DIR}/Engine/)
        target_include_directories(AGSWorks PUBLIC ${ags_content_SOURCE_DIR}/Common/)
    endif ()
endif ()

if(APPLE)
    # The MacOS version of AGS expects plugins to have the .dylib extension
    set_target_properties(AGSWorks PROPERTIES SUFFIX ".dylib")
endif()

if(MINGW OR MSYS)
    # Remove lib- prefix when compiling with MinGW
    set_target_properties(AGSWorks PROPERTIES PREFIX "")
endif()

target_sources(AGSWorks PRIVATE
        src/agsworks.cpp
        src/agsworks.h
        src/dlladapt.cpp
        src/dlladapt.h
        src/agsworksutil.h
        src/dynamicserviceloader.cpp
        src/dynamicserviceloader.h
        src/misc/base_library.h
        src/misc/utils.cpp
        src/misc/utils.h
        src/misc/library_windows.h
        src/misc/library_posix.h
        src/misc/library_posix.cpp
        src/misc/library_windows.cpp
        src/misc/library.h
        src/misc/awlog.cpp
        src/misc/awlog.h
        src/service/base/agsworksdriver.cpp
        src/service/base/agsworksdriver.h
        src/service/base/agswdummy.cpp
        src/service/base/agswdummy.h
        src/service/gog/agswgog.cpp
        src/service/gog/agswgog.h
        src/service/steam/agswsteam.cpp
        src/service/steam/agswsteam.h
        src/service/steam/dynapi_steam.cpp
        src/service/steam/dynapi_steam.h
)

install(TARGETS AGSWorks RUNTIME DESTINATION lib)
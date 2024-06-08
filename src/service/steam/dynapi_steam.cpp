//
// Created by erico on 6/7/2024.
//

#include "dynamicserviceloader.h"
#include "misc/awlog.h"
#include "dynapi_steam.h"


namespace AGSWorks
{


struct ISteamClient;
struct ISteamUserStats;
struct ISteamScreenshots;
struct CallbackMsg_t {
    int32_t m_hSteamUser;
    int32_t m_iCallback;
    uint8_t *m_pubParam;
    int32_t m_cubParam;
};
struct SteamAPICallCompleted_t {
    uint64_t m_hAsyncCall;
    int32_t m_iCallback;
    uint32_t m_cubParam;
};

static void *libHandle = nullptr;
static struct ISteamUserStats *steamUserStats = nullptr;
static struct ISteamScreenshots *steamScreenshots = nullptr;

static uint8_t (*SteamAPI_Init)() = nullptr;
static void (*SteamAPI_Shutdown)() = nullptr;
static void (*SteamAPI_RunCallbacks)() = nullptr;
static struct ISteamClient *(*SteamInternal_CreateInterface)(const char *) = nullptr;
static int32_t (*SteamAPI_GetHSteamUser)() = nullptr;
static int32_t (*SteamAPI_GetHSteamPipe)() = nullptr;
static struct ISteamUserStats * (*SteamAPI_ISteamClient_GetISteamUserStats)(struct ISteamClient *, int32_t, int32_t, const char *) = nullptr;
static uint8_t (*SteamAPI_ISteamUserStats_RequestCurrentStats)(struct ISteamUserStats *) = nullptr;
static uint8_t (*SteamAPI_ISteamUserStats_StoreStats)(struct ISteamUserStats *) = nullptr;
static uint8_t (*SteamAPI_ISteamUserStats_SetAchievement)(struct ISteamUserStats *, const char *) = nullptr;
static struct ISteamScreenshots * (*SteamAPI_ISteamClient_GetISteamScreenshots)(struct ISteamClient *, int32_t, int32_t, const char *) = nullptr;
static void (*SteamAPI_ISteamScreenshots_HookScreenshots)(struct ISteamScreenshots *, uint8_t) = nullptr;
static uint32_t (*SteamAPI_ISteamScreenshots_WriteScreenshot)(struct ISteamScreenshots *, void *, uint32_t, int32_t,
                                                              int32_t) = nullptr;
static void (*SteamAPI_ManualDispatch_Init)() = nullptr;
static void (*SteamAPI_ManualDispatch_RunFrame)(int32_t) = nullptr;
static uint8_t (*SteamAPI_ManualDispatch_GetNextCallback)(int32_t, struct CallbackMsg_t *) = nullptr;
static void (*SteamAPI_ManualDispatch_FreeLastCallback)(int32_t) = nullptr;
static uint8_t (*SteamAPI_ManualDispatch_GetAPICallResult)(int32_t, uint64_t, void *, int32_t, int32_t, uint8_t *) = nullptr;

static void *LoadFunc(const char* fn) {
    std::string sfn = fn;
    return DynamicServiceLoader::GetFunctionAddress(sfn);
}

static void CleanUp() {
    libHandle = nullptr;
    steamUserStats = nullptr;

    SteamAPI_Init = nullptr;
    SteamAPI_Shutdown = nullptr;
    SteamAPI_RunCallbacks = nullptr;
    SteamInternal_CreateInterface = nullptr;
    SteamAPI_GetHSteamUser = nullptr;
    SteamAPI_GetHSteamPipe = nullptr;
    SteamAPI_ISteamClient_GetISteamUserStats = nullptr;
    SteamAPI_ISteamUserStats_RequestCurrentStats = nullptr;
    SteamAPI_ISteamUserStats_StoreStats = nullptr;
    SteamAPI_ISteamUserStats_SetAchievement = nullptr;
    SteamAPI_ISteamClient_GetISteamScreenshots = nullptr;
    SteamAPI_ISteamScreenshots_HookScreenshots = nullptr;
    SteamAPI_ISteamScreenshots_WriteScreenshot = nullptr;
    SteamAPI_ManualDispatch_Init = nullptr;
    SteamAPI_ManualDispatch_RunFrame = nullptr;
    SteamAPI_ManualDispatch_GetNextCallback = nullptr;
    SteamAPI_ManualDispatch_FreeLastCallback = nullptr;
    SteamAPI_ManualDispatch_GetAPICallResult = nullptr;
}

static bool Init() {
    SteamAPI_Init = (uint8_t (*)(void)) (intptr_t) LoadFunc("SteamAPI_Init");
    if (!SteamAPI_Init) {
        AWLog::LogError("steam_api: symbol SteamAPI_Init not found!");
        CleanUp();
        return false;
    }
    SteamAPI_Shutdown = (void (*)(void)) (intptr_t) LoadFunc("SteamAPI_Shutdown");
    if (!SteamAPI_Shutdown) {
        AWLog::LogError("steam_api: symbol SteamAPI_Shutdown not found!");
        CleanUp();
        return false;
    }
    SteamAPI_RunCallbacks = (void (*)(void)) (intptr_t) LoadFunc("SteamAPI_RunCallbacks");
    if (!SteamAPI_RunCallbacks) {
        AWLog::LogError("steam_api: symbol SteamAPI_RunCallbacks not found!");
        CleanUp();
        return false;
    }
    SteamInternal_CreateInterface = (struct ISteamClient *(*)(const char *)) (intptr_t) LoadFunc("SteamInternal_CreateInterface");
    if (!SteamInternal_CreateInterface) {
        AWLog::LogError("steam_api: symbol SteamInternal_CreateInterface not found!");
        CleanUp();
        return false;
    }
    SteamAPI_GetHSteamUser = (int32_t (*)(void)) (intptr_t) LoadFunc("SteamAPI_GetHSteamUser");
    if (!SteamAPI_GetHSteamUser) {
        AWLog::LogError("steam_api: symbol SteamAPI_GetHSteamUser not found!");
        CleanUp();
        return false;
    }
    SteamAPI_GetHSteamPipe = (int32_t (*)(void)) (intptr_t) LoadFunc("SteamAPI_GetHSteamPipe");
    if (!SteamAPI_GetHSteamPipe) {
        AWLog::LogError("steam_api: symbol SteamAPI_GetHSteamPipe not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ISteamClient_GetISteamUserStats = (struct ISteamUserStats *(*)(struct ISteamClient *, int32_t, int32_t,
                                                                            const char *)) (intptr_t) LoadFunc("SteamAPI_ISteamClient_GetISteamUserStats");
    if (!SteamAPI_ISteamClient_GetISteamUserStats) {
        AWLog::LogError("steam_api: symbol SteamAPI_ISteamClient_GetISteamUserStats not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ISteamUserStats_RequestCurrentStats = (uint8_t (*)(
            struct ISteamUserStats *)) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_RequestCurrentStats");
    if (!SteamAPI_ISteamUserStats_RequestCurrentStats) {
        AWLog::LogError("steam_api: symbol SteamAPI_ISteamUserStats_RequestCurrentStats not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ISteamUserStats_StoreStats = (uint8_t (*)(struct ISteamUserStats *)) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_StoreStats");
    if (!SteamAPI_ISteamUserStats_StoreStats) {
        AWLog::LogError("steam_api: symbol SteamAPI_ISteamUserStats_StoreStats not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ISteamUserStats_SetAchievement = (uint8_t (*)(struct ISteamUserStats *,
                                                           const char *)) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_SetAchievement");
    if (!SteamAPI_ISteamUserStats_SetAchievement) {
        AWLog::LogError("steam_api: symbol SteamAPI_ISteamUserStats_SetAchievement not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ISteamClient_GetISteamScreenshots = (struct ISteamScreenshots *(*)(struct ISteamClient *, int32_t,
                                                                                int32_t,
                                                                                const char *)) (intptr_t) LoadFunc("SteamAPI_ISteamClient_GetISteamScreenshots");
    if (!SteamAPI_ISteamClient_GetISteamScreenshots) {
        AWLog::LogError("steam_api: symbol SteamAPI_ISteamClient_GetISteamScreenshots not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ISteamScreenshots_HookScreenshots = (void (*)(struct ISteamScreenshots *,
                                                           uint8_t)) (intptr_t) LoadFunc("SteamAPI_ISteamScreenshots_HookScreenshots");
    if (!SteamAPI_ISteamScreenshots_HookScreenshots) {
        AWLog::LogError("steam_api: symbol SteamAPI_ISteamScreenshots_HookScreenshots not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ISteamScreenshots_WriteScreenshot = (uint32_t (*)(struct ISteamScreenshots *, void *, uint32_t,
                                                               int32_t, int32_t)) (intptr_t) LoadFunc("SteamAPI_ISteamScreenshots_WriteScreenshot");
    if (!SteamAPI_ISteamScreenshots_WriteScreenshot) {
        AWLog::LogError("steam_api: symbol SteamAPI_ISteamScreenshots_WriteScreenshot not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ManualDispatch_Init = (void (*)(void)) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_Init");
    if (!SteamAPI_ManualDispatch_Init) {
        AWLog::LogError("steam_api: symbol SteamAPI_ManualDispatch_Init not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ManualDispatch_RunFrame = (void (*)(int32_t)) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_RunFrame");
    if (!SteamAPI_ManualDispatch_RunFrame) {
        AWLog::LogError("steam_api: symbol SteamAPI_ManualDispatch_RunFrame not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ManualDispatch_GetNextCallback = (uint8_t (*)(int32_t,
                                                           struct CallbackMsg_t *)) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_GetNextCallback");
    if (!SteamAPI_ManualDispatch_GetNextCallback) {
        AWLog::LogError("steam_api: symbol SteamAPI_ManualDispatch_GetNextCallback not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ManualDispatch_FreeLastCallback = (void (*)(int32_t)) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_FreeLastCallback");
    if (!SteamAPI_ManualDispatch_FreeLastCallback) {
        AWLog::LogError("steam_api: symbol SteamAPI_ManualDispatch_FreeLastCallback not found!");
        CleanUp();
        return false;
    }
    SteamAPI_ManualDispatch_GetAPICallResult = (uint8_t (*)(int32_t, uint64_t, void *, int32_t, int32_t,
                                                            uint8_t *)) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_GetAPICallResult");
    if (!SteamAPI_ManualDispatch_GetAPICallResult) {
        AWLog::LogError("steam_api: symbol SteamAPI_ManualDispatch_GetAPICallResult not found!");
        CleanUp();
        return false;
    }
    return true;
}

} // namespace AGSWorks
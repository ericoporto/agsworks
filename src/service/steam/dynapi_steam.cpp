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

using C_STEAMAPI_INIT = uint8_t (*)();
static C_STEAMAPI_INIT SteamAPI_Init = nullptr;
using C_STEAMAPI_SHUTDOWN = void (*)();
static C_STEAMAPI_SHUTDOWN SteamAPI_Shutdown = nullptr;
using C_STEAMAPI_RUNCALLBACKS = void (*)();
static C_STEAMAPI_RUNCALLBACKS SteamAPI_RunCallbacks = nullptr;
using C_STEAMAPI_CREATEINTERFACE = struct ISteamClient *(*)(const char *);
static C_STEAMAPI_CREATEINTERFACE SteamInternal_CreateInterface = nullptr;
using C_STEAMAPI_GETHSTEAMUSER = int32_t (*)();
static C_STEAMAPI_GETHSTEAMUSER SteamAPI_GetHSteamUser = nullptr;
using C_STEAMAPI_GETHSTEAMPIPE = int32_t (*)();
static C_STEAMAPI_GETHSTEAMPIPE SteamAPI_GetHSteamPipe = nullptr;
using C_STEAMAPI_ISTEAMCLIENT_GETISTEAMUSERSTATS = struct ISteamUserStats * (*)(struct ISteamClient *, int32_t, int32_t, const char *);
static C_STEAMAPI_ISTEAMCLIENT_GETISTEAMUSERSTATS SteamAPI_ISteamClient_GetISteamUserStats = nullptr;
using C_STEAMAPI_ISTEAMUSERSTATS_REQUESTCURRENTSTATS = uint8_t (*)(struct ISteamUserStats *);
static C_STEAMAPI_ISTEAMUSERSTATS_REQUESTCURRENTSTATS SteamAPI_ISteamUserStats_RequestCurrentStats = nullptr;
using C_STEAMAPI_ISTEAMUSERSTATS_STORESTATS = bool (*)(struct ISteamUserStats *);
static C_STEAMAPI_ISTEAMUSERSTATS_STORESTATS SteamAPI_ISteamUserStats_StoreStats = nullptr;
using C_STEAMAPI_ISTEAMUSERSTATS_SETACHIEVEMENT = bool (*)(struct ISteamUserStats *, const char *);
static C_STEAMAPI_ISTEAMUSERSTATS_SETACHIEVEMENT SteamAPI_ISteamUserStats_SetAchievement = nullptr;
using C_STEAMAPI_ISTEAMUSERSTATS_GETACHIEVEMENT = bool (*)(struct ISteamUserStats *, const char *, bool *);
static C_STEAMAPI_ISTEAMUSERSTATS_GETACHIEVEMENT SteamAPI_ISteamUserStats_GetAchievement = nullptr;
using C_STEAMAPI_ISTEAMUSERSTATS_CLEARACHIEVEMENT = bool (*)(struct ISteamUserStats *, const char *, bool *);
static C_STEAMAPI_ISTEAMUSERSTATS_CLEARACHIEVEMENT SteamAPI_ISteamUserStats_ClearAchievement = nullptr;
using C_STEAMAPI_ISTEAMUSERSTATS_GETNUMACHIEVEMENTS = uint32_t (*)(struct ISteamUserStats *);
static C_STEAMAPI_ISTEAMUSERSTATS_GETNUMACHIEVEMENTS SteamAPI_ISteamUserStats_GetNumAchievements = nullptr;
using C_STEAMAPI_ISTEAMCLIENT_GETISTEAMSCREENSHOTS = struct ISteamScreenshots * (*)(struct ISteamClient *, int32_t, int32_t, const char *);
static C_STEAMAPI_ISTEAMCLIENT_GETISTEAMSCREENSHOTS SteamAPI_ISteamClient_GetISteamScreenshots = nullptr;
using C_STEAMAPI_ISTEAMSCREENSHOTS_HOOKSCREENSHOTS = void (*)(struct ISteamScreenshots *, uint8_t);
static C_STEAMAPI_ISTEAMSCREENSHOTS_HOOKSCREENSHOTS SteamAPI_ISteamScreenshots_HookScreenshots = nullptr;
using C_STEAMAPI_ISTEAMSCREENSHOTS_WRITESCREENSHOT = uint32_t (*)(struct ISteamScreenshots *, void *, uint32_t, int32_t, int32_t);
static C_STEAMAPI_ISTEAMSCREENSHOTS_WRITESCREENSHOT SteamAPI_ISteamScreenshots_WriteScreenshot = nullptr;
using C_STEAMAPI_MANUALDISPATCH_INIT = void (*)();
static C_STEAMAPI_MANUALDISPATCH_INIT SteamAPI_ManualDispatch_Init = nullptr;
using C_STEAMAPI_MANUALDISPATCH_RUNFRAME = void (*)(int32_t);
static C_STEAMAPI_MANUALDISPATCH_RUNFRAME SteamAPI_ManualDispatch_RunFrame = nullptr;
using C_STEAMAPI_MANUALDISPATCH_GETNEXTCALLBACK = uint8_t (*)(int32_t, struct CallbackMsg_t *);
static C_STEAMAPI_MANUALDISPATCH_GETNEXTCALLBACK SteamAPI_ManualDispatch_GetNextCallback = nullptr;
using C_STEAMAPI_MANUALDISPATCH_FREELASTCALLBACK = void (*)(int32_t);
static C_STEAMAPI_MANUALDISPATCH_FREELASTCALLBACK SteamAPI_ManualDispatch_FreeLastCallback = nullptr;
using C_STEAMAPI_MANUALDISPATCH_GETAPICALLRESULT =  uint8_t (*)(int32_t, uint64_t, void *, int32_t, int32_t, uint8_t *);
static C_STEAMAPI_MANUALDISPATCH_GETAPICALLRESULT SteamAPI_ManualDispatch_GetAPICallResult = nullptr;

static void CleanUp()
{
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
    SteamAPI_ISteamUserStats_GetAchievement = nullptr;
    SteamAPI_ISteamUserStats_ClearAchievement = nullptr;
    SteamAPI_ISteamUserStats_GetNumAchievements = nullptr;
    SteamAPI_ISteamClient_GetISteamScreenshots = nullptr;
    SteamAPI_ISteamScreenshots_HookScreenshots = nullptr;
    SteamAPI_ISteamScreenshots_WriteScreenshot = nullptr;
    SteamAPI_ManualDispatch_Init = nullptr;
    SteamAPI_ManualDispatch_RunFrame = nullptr;
    SteamAPI_ManualDispatch_GetNextCallback = nullptr;
    SteamAPI_ManualDispatch_FreeLastCallback = nullptr;
    SteamAPI_ManualDispatch_GetAPICallResult = nullptr;
}


static void *LoadFunc(const char* fn) {
    std::string sfn = fn;
    void* pfn = DynamicServiceLoader::GetFunctionAddress(sfn);
    if(!pfn) {
        std::string msg = "steam_api: symbol '" + sfn + "' not found!";
        AWLog::LogError(msg);
        CleanUp();
        return nullptr;
    }
    return pfn;
}

static bool Init() {
    SteamAPI_Init = (C_STEAMAPI_INIT) (intptr_t) LoadFunc("SteamAPI_Init");
    if (!SteamAPI_Init) {
        return false;
    }
    SteamAPI_Shutdown = (C_STEAMAPI_SHUTDOWN) (intptr_t) LoadFunc("SteamAPI_Shutdown");
    if (!SteamAPI_Shutdown) {
        return false;
    }
    SteamAPI_RunCallbacks = (C_STEAMAPI_RUNCALLBACKS) (intptr_t) LoadFunc("SteamAPI_RunCallbacks");
    if (!SteamAPI_RunCallbacks) {
        return false;
    }
    SteamInternal_CreateInterface = (C_STEAMAPI_CREATEINTERFACE) (intptr_t) LoadFunc("SteamInternal_CreateInterface");
    if (!SteamInternal_CreateInterface) {
        return false;
    }
    SteamAPI_GetHSteamUser = (C_STEAMAPI_GETHSTEAMUSER) (intptr_t) LoadFunc("SteamAPI_GetHSteamUser");
    if (!SteamAPI_GetHSteamUser) {
        return false;
    }
    SteamAPI_GetHSteamPipe = (C_STEAMAPI_GETHSTEAMPIPE) (intptr_t) LoadFunc("SteamAPI_GetHSteamPipe");
    if (!SteamAPI_GetHSteamPipe) {
        return false;
    }
    SteamAPI_ISteamClient_GetISteamUserStats = (C_STEAMAPI_ISTEAMCLIENT_GETISTEAMUSERSTATS) (intptr_t) LoadFunc("SteamAPI_ISteamClient_GetISteamUserStats");
    if (!SteamAPI_ISteamClient_GetISteamUserStats) {
        return false;
    }
    SteamAPI_ISteamUserStats_RequestCurrentStats = (C_STEAMAPI_ISTEAMUSERSTATS_REQUESTCURRENTSTATS) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_RequestCurrentStats");
    if (!SteamAPI_ISteamUserStats_RequestCurrentStats) {
        return false;
    }
    SteamAPI_ISteamUserStats_StoreStats = (C_STEAMAPI_ISTEAMUSERSTATS_STORESTATS) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_StoreStats");
    if (!SteamAPI_ISteamUserStats_StoreStats) {
        return false;
    }
    SteamAPI_ISteamUserStats_SetAchievement = (C_STEAMAPI_ISTEAMUSERSTATS_SETACHIEVEMENT) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_SetAchievement");
    if (!SteamAPI_ISteamUserStats_SetAchievement) {
        return false;
    }
    SteamAPI_ISteamUserStats_GetAchievement = (C_STEAMAPI_ISTEAMUSERSTATS_GETACHIEVEMENT) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_GetAchievement");
    if (!SteamAPI_ISteamUserStats_GetAchievement) {
        return false;
    }
    SteamAPI_ISteamUserStats_ClearAchievement = (C_STEAMAPI_ISTEAMUSERSTATS_CLEARACHIEVEMENT) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_ClearAchievement");
    if (!SteamAPI_ISteamUserStats_ClearAchievement) {
        return false;
    }
    SteamAPI_ISteamUserStats_GetNumAchievements = (C_STEAMAPI_ISTEAMUSERSTATS_GETNUMACHIEVEMENTS) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_GetNumAchievements");
    if (!SteamAPI_ISteamUserStats_GetNumAchievements) {
        return false;
    }
    SteamAPI_ISteamClient_GetISteamScreenshots = (C_STEAMAPI_ISTEAMCLIENT_GETISTEAMSCREENSHOTS) (intptr_t) LoadFunc("SteamAPI_ISteamClient_GetISteamScreenshots");
    if (!SteamAPI_ISteamClient_GetISteamScreenshots) {
        return false;
    }
    SteamAPI_ISteamScreenshots_HookScreenshots = (C_STEAMAPI_ISTEAMSCREENSHOTS_HOOKSCREENSHOTS) (intptr_t) LoadFunc("SteamAPI_ISteamScreenshots_HookScreenshots");
    if (!SteamAPI_ISteamScreenshots_HookScreenshots) {
        return false;
    }
    SteamAPI_ISteamScreenshots_WriteScreenshot = (C_STEAMAPI_ISTEAMSCREENSHOTS_WRITESCREENSHOT) (intptr_t) LoadFunc("SteamAPI_ISteamScreenshots_WriteScreenshot");
    if (!SteamAPI_ISteamScreenshots_WriteScreenshot) {
        return false;
    }
    SteamAPI_ManualDispatch_Init = (C_STEAMAPI_MANUALDISPATCH_INIT) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_Init");
    if (!SteamAPI_ManualDispatch_Init) {
        return false;
    }
    SteamAPI_ManualDispatch_RunFrame = (C_STEAMAPI_MANUALDISPATCH_RUNFRAME) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_RunFrame");
    if (!SteamAPI_ManualDispatch_RunFrame) {
        return false;
    }
    SteamAPI_ManualDispatch_GetNextCallback = (C_STEAMAPI_MANUALDISPATCH_GETNEXTCALLBACK) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_GetNextCallback");
    if (!SteamAPI_ManualDispatch_GetNextCallback) {
        return false;
    }
    SteamAPI_ManualDispatch_FreeLastCallback = (C_STEAMAPI_MANUALDISPATCH_FREELASTCALLBACK) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_FreeLastCallback");
    if (!SteamAPI_ManualDispatch_FreeLastCallback) {
        return false;
    }
    SteamAPI_ManualDispatch_GetAPICallResult = (C_STEAMAPI_MANUALDISPATCH_GETAPICALLRESULT) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_GetAPICallResult");
    if (!SteamAPI_ManualDispatch_GetAPICallResult) {
        return false;
    }
    return true;
}

} // namespace AGSWorks
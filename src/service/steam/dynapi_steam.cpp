//
// Created by erico on 6/7/2024.
//

#include "dynamicserviceloader.h"
#include "misc/awlog.h"
#include "dynapi_steam.h"


namespace AGSWorks
{

static void *libHandle = nullptr;
static struct ISteamUserStats *steamUserStats = nullptr;
static struct ISteamScreenshots *steamScreenshots = nullptr;


void DynApiSteam::ClearFuncsToNull()
{
    libHandle = nullptr;
    steamUserStats = nullptr;
    c_SteamAPI_InitFlat = nullptr;
    c_SteamAPI_Shutdown = nullptr;
    c_SteamAPI_RunCallbacks = nullptr;
    c_SteamInternal_CreateInterface = nullptr;
    c_SteamAPI_GetHSteamUser = nullptr;
    c_SteamAPI_GetHSteamPipe = nullptr;
    c_SteamAPI_ISteamClient_GetISteamUserStats = nullptr;
    c_SteamAPI_ISteamUserStats_RequestCurrentStats = nullptr;
    c_SteamAPI_ISteamUserStats_StoreStats = nullptr;
    c_SteamAPI_ISteamUserStats_SetAchievement = nullptr;
    c_SteamAPI_ISteamUserStats_GetAchievement = nullptr;
    c_SteamAPI_ISteamUserStats_ClearAchievement = nullptr;
    c_SteamAPI_ISteamUserStats_GetNumAchievements = nullptr;
    c_SteamAPI_ISteamUserStats_ResetAllStats = nullptr;
    c_SteamAPI_ISteamClient_GetISteamScreenshots = nullptr;
    c_SteamAPI_ISteamScreenshots_HookScreenshots = nullptr;
    c_SteamAPI_ISteamScreenshots_WriteScreenshot = nullptr;
    c_SteamAPI_ManualDispatch_Init = nullptr;
    c_SteamAPI_ManualDispatch_RunFrame = nullptr;
    c_SteamAPI_ManualDispatch_GetNextCallback = nullptr;
    c_SteamAPI_ManualDispatch_FreeLastCallback = nullptr;
    c_SteamAPI_ManualDispatch_GetAPICallResult = nullptr;
}

void DynApiSteam::Shutdown()
{
    ClearFuncsToNull();
}


void * DynApiSteam::LoadFunc(const char* fn) {
    std::string sfn = fn;
    void* pfn = DynamicServiceLoader::GetFunctionAddress(sfn);
    if(!pfn) {
        std::string msg = "steam_api: symbol '" + sfn + "' not found!";
        AWLog::LogError(msg);
        Shutdown();
        return nullptr;
    }
    return pfn;
}

bool DynApiSteam::Init() {
    ClearFuncsToNull();
    c_SteamAPI_InitFlat = (C_STEAMAPI_INITFLAT ) (intptr_t) LoadFunc("SteamAPI_InitFlat");
    if (!c_SteamAPI_InitFlat) {
        return false;
    }
    c_SteamAPI_Shutdown = (C_STEAMAPI_SHUTDOWN) (intptr_t) LoadFunc("SteamAPI_Shutdown");
    if (!c_SteamAPI_Shutdown) {
        return false;
    }
    c_SteamAPI_RunCallbacks = (C_STEAMAPI_RUNCALLBACKS) (intptr_t) LoadFunc("SteamAPI_RunCallbacks");
    if (!c_SteamAPI_RunCallbacks) {
        return false;
    }
    c_SteamInternal_CreateInterface = (C_STEAMINTERNAL_CREATEINTERFACE) (intptr_t) LoadFunc("SteamInternal_CreateInterface");
    if (!c_SteamInternal_CreateInterface) {
        return false;
    }
    c_SteamAPI_GetHSteamUser = (C_STEAMAPI_GETHSTEAMUSER) (intptr_t) LoadFunc("SteamAPI_GetHSteamUser");
    if (!c_SteamAPI_GetHSteamUser) {
        return false;
    }
    c_SteamAPI_GetHSteamPipe = (C_STEAMAPI_GETHSTEAMPIPE) (intptr_t) LoadFunc("SteamAPI_GetHSteamPipe");
    if (!c_SteamAPI_GetHSteamPipe) {
        return false;
    }
    c_SteamAPI_ISteamClient_GetISteamUserStats = (C_STEAMAPI_ISTEAMCLIENT_GETISTEAMUSERSTATS) (intptr_t) LoadFunc("SteamAPI_ISteamClient_GetISteamUserStats");
    if (!c_SteamAPI_ISteamClient_GetISteamUserStats) {
        return false;
    }
    c_SteamAPI_ISteamUserStats_RequestCurrentStats = (C_STEAMAPI_ISTEAMUSERSTATS_REQUESTCURRENTSTATS) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_RequestCurrentStats");
    if (!c_SteamAPI_ISteamUserStats_RequestCurrentStats) {
        return false;
    }
    c_SteamAPI_ISteamUserStats_StoreStats = (C_STEAMAPI_ISTEAMUSERSTATS_STORESTATS) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_StoreStats");
    if (!c_SteamAPI_ISteamUserStats_StoreStats) {
        return false;
    }
    c_SteamAPI_ISteamUserStats_SetAchievement = (C_STEAMAPI_ISTEAMUSERSTATS_SETACHIEVEMENT) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_SetAchievement");
    if (!c_SteamAPI_ISteamUserStats_SetAchievement) {
        return false;
    }
    c_SteamAPI_ISteamUserStats_GetAchievement = (C_STEAMAPI_ISTEAMUSERSTATS_GETACHIEVEMENT) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_GetAchievement");
    if (!c_SteamAPI_ISteamUserStats_GetAchievement) {
        return false;
    }
    c_SteamAPI_ISteamUserStats_ClearAchievement = (C_STEAMAPI_ISTEAMUSERSTATS_CLEARACHIEVEMENT) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_ClearAchievement");
    if (!c_SteamAPI_ISteamUserStats_ClearAchievement) {
        return false;
    }
    c_SteamAPI_ISteamUserStats_GetNumAchievements = (C_STEAMAPI_ISTEAMUSERSTATS_GETNUMACHIEVEMENTS) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_GetNumAchievements");
    if (!c_SteamAPI_ISteamUserStats_GetNumAchievements) {
        return false;
    }
    c_SteamAPI_ISteamUserStats_ResetAllStats = (C_STEAMAPI_ISTEAMUSERSTATS_RESETALLSTATS) (intptr_t) LoadFunc("SteamAPI_ISteamUserStats_ResetAllStats");
    if (!c_SteamAPI_ISteamUserStats_ResetAllStats) {
        return false;
    }
    c_SteamAPI_ISteamClient_GetISteamScreenshots = (C_STEAMAPI_ISTEAMCLIENT_GETISTEAMSCREENSHOTS) (intptr_t) LoadFunc("SteamAPI_ISteamClient_GetISteamScreenshots");
    if (!c_SteamAPI_ISteamClient_GetISteamScreenshots) {
        return false;
    }
    c_SteamAPI_ISteamScreenshots_HookScreenshots = (C_STEAMAPI_ISTEAMSCREENSHOTS_HOOKSCREENSHOTS) (intptr_t) LoadFunc("SteamAPI_ISteamScreenshots_HookScreenshots");
    if (!c_SteamAPI_ISteamScreenshots_HookScreenshots) {
        return false;
    }
    c_SteamAPI_ISteamScreenshots_WriteScreenshot = (C_STEAMAPI_ISTEAMSCREENSHOTS_WRITESCREENSHOT) (intptr_t) LoadFunc("SteamAPI_ISteamScreenshots_WriteScreenshot");
    if (!c_SteamAPI_ISteamScreenshots_WriteScreenshot) {
        return false;
    }
    c_SteamAPI_ManualDispatch_Init = (C_STEAMAPI_MANUALDISPATCH_INIT) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_Init");
    if (!c_SteamAPI_ManualDispatch_Init) {
        return false;
    }
    c_SteamAPI_ManualDispatch_RunFrame = (C_STEAMAPI_MANUALDISPATCH_RUNFRAME) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_RunFrame");
    if (!c_SteamAPI_ManualDispatch_RunFrame) {
        return false;
    }
    c_SteamAPI_ManualDispatch_GetNextCallback = (C_STEAMAPI_MANUALDISPATCH_GETNEXTCALLBACK) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_GetNextCallback");
    if (!c_SteamAPI_ManualDispatch_GetNextCallback) {
        return false;
    }
    c_SteamAPI_ManualDispatch_FreeLastCallback = (C_STEAMAPI_MANUALDISPATCH_FREELASTCALLBACK) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_FreeLastCallback");
    if (!c_SteamAPI_ManualDispatch_FreeLastCallback) {
        return false;
    }
    c_SteamAPI_ManualDispatch_GetAPICallResult = (C_STEAMAPI_MANUALDISPATCH_GETAPICALLRESULT) (intptr_t) LoadFunc("SteamAPI_ManualDispatch_GetAPICallResult");
    if (!c_SteamAPI_ManualDispatch_GetAPICallResult) {
        return false;
    }
    return true;
}

ESteamAPIInitResult DynApiSteam::SteamAPI_InitFlat(SteamErrMsg *pOutErrMsg) {
    return static_cast<ESteamAPIInitResult>(c_SteamAPI_InitFlat(pOutErrMsg));
}

void DynApiSteam::SteamAPI_Shutdown() {
    c_SteamAPI_Shutdown();
}

void DynApiSteam::SteamAPI_RunCallbacks() {
    c_SteamAPI_RunCallbacks();
}

bool DynApiSteam::SteamAPI_ISteamUserStats_StoreStats(ISteamUserStats *self) {
    return c_SteamAPI_ISteamUserStats_StoreStats(self);
}

bool DynApiSteam::SteamAPI_ISteamUserStats_SetAchievement(ISteamUserStats *self, const char *pchName) {
    return c_SteamAPI_ISteamUserStats_SetAchievement(self, pchName);
}

bool DynApiSteam::SteamAPI_ISteamUserStats_GetAchievement(ISteamUserStats *self, const char *pchName, bool *pbAchieved) {
    return c_SteamAPI_ISteamUserStats_GetAchievement(self, pchName, pbAchieved);
}

bool DynApiSteam::SteamAPI_ISteamUserStats_ClearAchievement(ISteamUserStats *self, const char *pchName) {
    return c_SteamAPI_ISteamUserStats_ClearAchievement(self, pchName);
}

uint32_t DynApiSteam::SteamAPI_ISteamUserStats_GetNumAchievements(ISteamUserStats *self) {
    return c_SteamAPI_ISteamUserStats_GetNumAchievements(self);
}

bool DynApiSteam::SteamAPI_ISteamUserStats_RequestCurrentStats(ISteamUserStats *self) {
    return c_SteamAPI_ISteamUserStats_RequestCurrentStats(self);
}

ISteamUserStats *DynApiSteam::SteamAPI_ISteamClient_GetISteamUserStats(ISteamClient *self, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char *pchVersion) {
    return c_SteamAPI_ISteamClient_GetISteamUserStats(self, hSteamUser, hSteamPipe, pchVersion);
}

void DynApiSteam::SteamAPI_ManualDispatch_Init() {
    c_SteamAPI_ManualDispatch_Init();
}

ISteamClient *DynApiSteam::SteamInternal_CreateInterface(const char *ver) {
    return c_SteamInternal_CreateInterface(ver);
}

HSteamUser DynApiSteam::SteamAPI_GetHSteamUser() {
    return c_SteamAPI_GetHSteamUser();
}

HSteamPipe DynApiSteam::SteamAPI_GetHSteamPipe() {
    return c_SteamAPI_GetHSteamPipe();
}

bool DynApiSteam::SteamAPI_ISteamUserStats_ResetAllStats(ISteamUserStats *self, bool bAchievementsToo) {
    return c_SteamAPI_ISteamUserStats_ResetAllStats(self, bAchievementsToo);
}


} // namespace AGSWorks
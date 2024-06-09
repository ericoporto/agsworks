//
// Created by erico on 6/7/2024.
//

#ifndef AGSWORKS_DYNAPI_STEAM_H
#define AGSWORKS_DYNAPI_STEAM_H

#include "min_steam_api.h"

namespace AGSWorks {
    class DynApiSteam {
    private:
        void* LoadFunc(const char* fn);
        void ClearFuncsToNull();

        using C_STEAMAPI_INITFLAT = int32_t (*)(SteamErrMsg *);
        C_STEAMAPI_INITFLAT c_SteamAPI_InitFlat;
        using C_STEAMAPI_SHUTDOWN = void (*)();
        C_STEAMAPI_SHUTDOWN c_SteamAPI_Shutdown;
        using C_STEAMAPI_RUNCALLBACKS = void (*)();
        C_STEAMAPI_RUNCALLBACKS c_SteamAPI_RunCallbacks;
        using C_STEAMINTERNAL_CREATEINTERFACE = struct ISteamClient *(*)(const char *);
        C_STEAMINTERNAL_CREATEINTERFACE c_SteamInternal_CreateInterface;
        using C_STEAMAPI_GETHSTEAMUSER = int32_t (*)();
        C_STEAMAPI_GETHSTEAMUSER c_SteamAPI_GetHSteamUser;
        using C_STEAMAPI_GETHSTEAMPIPE = int32_t (*)();
        C_STEAMAPI_GETHSTEAMPIPE c_SteamAPI_GetHSteamPipe;
        using C_STEAMAPI_ISTEAMCLIENT_GETISTEAMUSERSTATS = struct ISteamUserStats * (*)(struct ISteamClient *, int32_t, int32_t, const char *);
        C_STEAMAPI_ISTEAMCLIENT_GETISTEAMUSERSTATS c_SteamAPI_ISteamClient_GetISteamUserStats;
        using C_STEAMAPI_ISTEAMUSERSTATS_REQUESTCURRENTSTATS = uint8_t (*)(struct ISteamUserStats *);
        C_STEAMAPI_ISTEAMUSERSTATS_REQUESTCURRENTSTATS c_SteamAPI_ISteamUserStats_RequestCurrentStats;
        using C_STEAMAPI_ISTEAMUSERSTATS_STORESTATS = bool (*)(struct ISteamUserStats *);
        C_STEAMAPI_ISTEAMUSERSTATS_STORESTATS c_SteamAPI_ISteamUserStats_StoreStats;
        using C_STEAMAPI_ISTEAMUSERSTATS_SETACHIEVEMENT = bool (*)(struct ISteamUserStats *, const char *);
        C_STEAMAPI_ISTEAMUSERSTATS_SETACHIEVEMENT c_SteamAPI_ISteamUserStats_SetAchievement;
        using C_STEAMAPI_ISTEAMUSERSTATS_GETACHIEVEMENT = bool (*)(struct ISteamUserStats *, const char *, bool *);
        C_STEAMAPI_ISTEAMUSERSTATS_GETACHIEVEMENT c_SteamAPI_ISteamUserStats_GetAchievement;
        using C_STEAMAPI_ISTEAMUSERSTATS_CLEARACHIEVEMENT = bool (*)(struct ISteamUserStats *, const char *);
        C_STEAMAPI_ISTEAMUSERSTATS_CLEARACHIEVEMENT c_SteamAPI_ISteamUserStats_ClearAchievement;
        using C_STEAMAPI_ISTEAMUSERSTATS_GETNUMACHIEVEMENTS = uint32_t (*)(struct ISteamUserStats *);
        C_STEAMAPI_ISTEAMUSERSTATS_GETNUMACHIEVEMENTS c_SteamAPI_ISteamUserStats_GetNumAchievements;
        using C_STEAMAPI_ISTEAMUSERSTATS_RESETALLSTATS = bool (*)(ISteamUserStats*, bool);
        C_STEAMAPI_ISTEAMUSERSTATS_RESETALLSTATS c_SteamAPI_ISteamUserStats_ResetAllStats;
        using C_STEAMAPI_ISTEAMCLIENT_GETISTEAMSCREENSHOTS = struct ISteamScreenshots * (*)(struct ISteamClient *, int32_t, int32_t, const char *);
        C_STEAMAPI_ISTEAMCLIENT_GETISTEAMSCREENSHOTS c_SteamAPI_ISteamClient_GetISteamScreenshots;
        using C_STEAMAPI_ISTEAMSCREENSHOTS_HOOKSCREENSHOTS = void (*)(struct ISteamScreenshots *, uint8_t);
        C_STEAMAPI_ISTEAMSCREENSHOTS_HOOKSCREENSHOTS c_SteamAPI_ISteamScreenshots_HookScreenshots;
        using C_STEAMAPI_ISTEAMSCREENSHOTS_WRITESCREENSHOT = uint32_t (*)(struct ISteamScreenshots *, void *, uint32_t, int32_t, int32_t);
        C_STEAMAPI_ISTEAMSCREENSHOTS_WRITESCREENSHOT c_SteamAPI_ISteamScreenshots_WriteScreenshot;
        using C_STEAMAPI_MANUALDISPATCH_INIT = void (*)();
        C_STEAMAPI_MANUALDISPATCH_INIT c_SteamAPI_ManualDispatch_Init;
        using C_STEAMAPI_MANUALDISPATCH_RUNFRAME = void (*)(int32_t);
        C_STEAMAPI_MANUALDISPATCH_RUNFRAME c_SteamAPI_ManualDispatch_RunFrame;
        using C_STEAMAPI_MANUALDISPATCH_GETNEXTCALLBACK = uint8_t (*)(int32_t, struct CallbackMsg_t *);
        C_STEAMAPI_MANUALDISPATCH_GETNEXTCALLBACK c_SteamAPI_ManualDispatch_GetNextCallback;
        using C_STEAMAPI_MANUALDISPATCH_FREELASTCALLBACK = void (*)(int32_t);
        C_STEAMAPI_MANUALDISPATCH_FREELASTCALLBACK c_SteamAPI_ManualDispatch_FreeLastCallback;
        using C_STEAMAPI_MANUALDISPATCH_GETAPICALLRESULT =  uint8_t (*)(int32_t, uint64_t, void *, int32_t, int32_t, bool *);
        C_STEAMAPI_MANUALDISPATCH_GETAPICALLRESULT c_SteamAPI_ManualDispatch_GetAPICallResult;

    public:
        bool Init();
        void Shutdown();

        ESteamAPIInitResult SteamAPI_InitFlat( SteamErrMsg *pOutErrMsg );
        void SteamAPI_Shutdown();
        void SteamAPI_RunCallbacks();
        bool SteamAPI_ISteamUserStats_StoreStats(ISteamUserStats* self);
        bool SteamAPI_ISteamUserStats_SetAchievement(ISteamUserStats* self, const char * pchName);
        bool SteamAPI_ISteamUserStats_GetAchievement(ISteamUserStats* self, const char * pchName, bool * pbAchieved);
        bool SteamAPI_ISteamUserStats_ClearAchievement(ISteamUserStats* self, const char * pchName);
        uint32_t SteamAPI_ISteamUserStats_GetNumAchievements(ISteamUserStats* self );
        bool SteamAPI_ISteamUserStats_RequestCurrentStats(ISteamUserStats* self );
        bool SteamAPI_ISteamUserStats_ResetAllStats(ISteamUserStats* self, bool bAchievementsToo);
        ISteamUserStats * SteamAPI_ISteamClient_GetISteamUserStats(ISteamClient* self, HSteamUser hSteamUser, HSteamPipe hSteamPipe, const char * pchVersion);
        void SteamAPI_ManualDispatch_Init();
        ISteamClient *SteamInternal_CreateInterface(const char *string);
        HSteamUser SteamAPI_GetHSteamUser();
        HSteamPipe SteamAPI_GetHSteamPipe();
        void SteamAPI_ManualDispatch_RunFrame(HSteamPipe hSteamPipe);
        bool SteamAPI_ManualDispatch_GetNextCallback(HSteamPipe hSteamPipe, CallbackMsg_t *pCallbackMsg);
        void SteamAPI_ManualDispatch_FreeLastCallback(HSteamPipe hSteamPipe );
        bool SteamAPI_ManualDispatch_GetAPICallResult(HSteamPipe hSteamPipe, SteamAPICall_t hSteamAPICall, void *pCallback, int cubCallback, int iCallbackExpected, bool *pbFailed);
    };

}

#endif //AGSWORKS_DYNAPI_STEAM_H

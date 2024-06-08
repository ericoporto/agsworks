
#include "agswsteam.h"
#include "misc/awlog.h"

namespace AGSWorks
{

void SteamWorksDriver::Init()
{
    _s = DynApiSteam();
    _s.Init();

    SteamErrMsg errMsg;
    ESteamAPIInitResult ini_res = _s.SteamAPI_InitFlat(&errMsg);
    if (ini_res != ESteamAPIInitResult::k_ESteamAPIInitResult_OK)
    {
        std::string emsg = "SteamWorksDriver::Init error: ";
        emsg += errMsg;
        AWLog::LogError(emsg);
        _s.Shutdown();
        return;
    }

    _s.SteamAPI_ManualDispatch_Init();

    _steamClient = _s.SteamInternal_CreateInterface(STEAMCLIENT_INTERFACE_VERSION);
    HSteamUser steamUser = _s.SteamAPI_GetHSteamUser();
    HSteamPipe steamPipe = _s.SteamAPI_GetHSteamPipe();
    if (!_steamClient || !steamUser || !steamPipe)
    {
        _s.SteamAPI_Shutdown();
        AWLog::LogError("SteamWorksDriver::Init error: ISteamClient Interface was not created!");
        _s.Shutdown();
        return;
    }

    _steamUserStats = _s.SteamAPI_ISteamClient_GetISteamUserStats(
            _steamClient,
            steamUser,
            steamPipe,
            STEAMUSERSTATS_INTERFACE_VERSION
    );
    if (!_steamUserStats)
    {
        _s.SteamAPI_Shutdown();
        AWLog::LogError("SteamWorksDriver::Init error: ISteamUserStats Interface was not created!");
        _s.Shutdown();
        return;
    }
    _s.SteamAPI_ISteamUserStats_RequestCurrentStats(_steamUserStats);

    _initialized = true;
}

void SteamWorksDriver::Shutdown()
{
    if(_initialized) {
        _s.SteamAPI_Shutdown();
        _s.Shutdown();
    }
}

SteamWorksDriver* SteamWorksDriver::CreateDriver()
{
    return new SteamWorksDriver();
}

ServiceType SteamWorksDriver::GetServiceType()
{
    return ServiceType::eSteam;
}

} // namespace AGSWorks

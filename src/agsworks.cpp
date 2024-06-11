/*
 * MIT License
 *
 * Copyright (c) 2024 Érico Porto
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <string>
#include <vector>
#include "agsworks.h"
#include "agsworksutil.h"
#include "dlladapt.h"
#include "service/base/agsworksdriver.h"
#include "dynamicserviceloader.h"
#include "misc/awlog.h"

using namespace AGSWorks;
DllAdapt dllAdaptor;

#ifdef _WIN32

// DllMain - standard Windows DLL entry point.
// The AGS editor will cause this to get called when the editor first
// starts up, and when it shuts down at the end.
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH: {
            dllAdaptor.SetModule(hModule);
        }
            break;
    }
    return TRUE;
}
#define LIBRARY_API
#define EXPORT comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__)
#else
#define LIBRARY_API extern "C"
#define EXPORT
#endif // _WIN32

// ***** DESIGN TIME CALLS *******
IAGSEditor *editor = nullptr;
const char *ourScriptHeader =
      "#define AGSWORKS_VERSION 1\r\n"
      "\r\n"
      "enum AgsWorksStatType\r\n"
      "{\r\n"
      "  eAgsWorksStatInt = 0,\r\n"
      "  eAgsWorksStatFloat = 1,\r\n"
      "  eAgsWorksStatAverageRate = 2\r\n"
      "};\r\n"
      "\r\n"
      "enum AgsWorksScoresRequestType\r\n"
      "{\r\n"
      "  eAgsWorksScoresRequestGlobal = 0,\r\n"
      "  eAgsWorksScoresRequestAroundUser = 1,\r\n"
      "  eAgsWorksScoresRequestFriends = 2\r\n"
      "};\r\n"
      "\r\n"
      "builtin struct AgsWorksCompat\r\n"
      "{\r\n"
      "  /// Returns whether the specified achievement has been achieved\r\n"
      "  import static int IsAchievementAchieved(const string name);\r\n"
      "  /// Sets an achievement as achieved\r\n"
      "  import static int SetAchievementAchieved(const string name);\r\n"
      "  /// Resets the specified achievement, so it can be achieved again\r\n"
      "  import static int ResetAchievement(const string name);\r\n"
      "  /// Sets the value of an INT stat\r\n"
      "  import static int SetIntStat(const string name, int value);\r\n"
      "  /// Sets the value of a FLOAT stat\r\n"
      "  import static int SetFloatStat(const string name, float value);\r\n"
      "  /// Updates an average rate stat\r\n"
      "  import static int UpdateAverageRateStat(const string name, float countThisSession, float sessionLength);\r\n"
      "  /// Returns the value of an INT stat\r\n"
      "  import static int GetIntStat(const string name);\r\n"
      "  /// Returns the value of a FLOAT stat\r\n"
      "  import static float GetFloatStat(const string name);\r\n"
      "  /// Returns the value of an average rate stat\r\n"
      "  import static float GetAverageRateStat(const string name);\r\n"
      "  /// Resets all stats and achievements\r\n"
      "  import static void ResetStatsAndAchievements();\r\n"
      "  /// Returns whether the client is loaded and the user logged in\r\n"
      "  readonly import static attribute int Initialized;\r\n"
      "  /// Returns the name of the current leaderboard (call FindLeaderboard first)\r\n"
      "  readonly import static attribute String CurrentLeaderboardName;\r\n"
      "  /// Requests to load the specified leaderboard. This call is asynchronous and does not return the data immediately, check for results in repeatedly_execute\r\n"
      "  import static void RequestLeaderboard(const string leaderboardName, AgsWorksScoresRequestType, int limit=10);\r\n"
      "  /// Uploads the score to the current leaderboard (call RequestLeaderboard first). Returns false if an error occurs\r\n"
      "  import static int UploadScore(int score);\r\n"
      "  /// Returns the name associated with a leaderboard rank (call RequestLeaderboard first)\r\n"
      "  readonly import static attribute String LeaderboardNames[];\r\n"
      "  /// Returns a leader's score from the current leaderboard (call RequestLeaderboard first)\r\n"
      "  readonly import static attribute int LeaderboardScores[];\r\n"
      "  /// Returns the number of leaders in the current leaderboard (if any) (call RequestLeaderboard first)\r\n"
      "  readonly import static attribute int LeaderboardCount;\r\n"
      "  /// Returns the username from the client\r\n"
      "  import static String GetUserName();\r\n"
      "  /// Returns the current game language from the client\r\n"
      "  import static String GetCurrentGameLanguage();\r\n"
      "  /// Returns the current game language from the client\r\n"
      "  import static int FindLeaderboard(const string leaderboard_name);\r\n"
      "};\r\n"
      "\r\n";

LIBRARY_API int AGS_EditorStartup(IAGSEditor *lpEditor)
{
    #pragma EXPORT
    if (lpEditor->version < 1)
        return -1;

    editor = lpEditor;
    editor->RegisterScriptHeader(ourScriptHeader);
    return 0; // Return 0 to indicate success
}

LIBRARY_API void AGS_EditorShutdown()
{
    #pragma EXPORT
    editor->UnregisterScriptHeader(ourScriptHeader);
}

LIBRARY_API void AGS_EditorProperties(HWND parent)
{
    #pragma EXPORT
}

LIBRARY_API int AGS_EditorSaveGame(char *buffer, int bufsize)
{
    #pragma EXPORT
    return 0; 
}

LIBRARY_API void AGS_EditorLoadGame(char *buffer, int bufsize)
{
    #pragma EXPORT
}

// ******* END DESIGN TIME  *******

// ****** RUN TIME ********
enum AgsWorksStatType {
    eAgsWorksStatInt = 0,
    eAgsWorksStatFloat = 1,
    eAgsWorksStatAverageRate = 2
};
enum AgsWorksScoresRequestType {
    eAgsWorksScoresRequestGlobal = 0,
    eAgsWorksScoresRequestAroundUser = 1,
    eAgsWorksScoresRequestFriends = 2
};
IAGSEngine *engine = nullptr;
AGSWorksDriver* works = nullptr;

#define FAIL_LOG_AND_EXIT(X) \
do {                    \
  aw_log_info((X));   \
  return 0;             \
} while (0)

union
{
    float f;
    uint32_t ui32;
} AgsNumber;

uint32_t inline ToAgsFloat(float f) {
    AgsNumber.f = f;
    return AgsNumber.ui32;
}

float inline ToNormalFloat(uint32_t ui32) {
    AgsNumber.ui32 = ui32;
    return AgsNumber.f;
}

int inline ToAgsBool(bool b){
    return b ? 1 : 0;
}

bool inline ToNormalBool(int bi){
    return bi != 0;
}

bool inline IsEmpty(const char* txt)
{
    return txt == nullptr || txt[0] == '\0';
}

void aw_log_info(const std::string& message){
    if(engine == nullptr) return;
    engine->PrintDebugConsole(message.c_str());
}

void AgsWorksCompat_Initialize(const char *clientID, const char *clientSecret)
{
    if(works) {
        works->AgsWorksCompat_Initialize(clientID, clientSecret);
    }
}

int AgsWorksCompat_IsAchievementAchieved(const char* achievment_name)
{
    if(!IsEmpty(achievment_name) && works) {
        return ToAgsBool(works->AgsWorksCompat_IsAchievementAchieved(achievment_name));
    }
    return 0;
}

int AgsWorksCompat_SetAchievementAchieved(const char* achievment_name)
{
    if(!IsEmpty(achievment_name) && works) {
        return ToAgsBool(works->AgsWorksCompat_SetAchievementAchieved(achievment_name));
    }
    return 0;
}

int AgsWorksCompat_ResetAchievement(const char* achievment_name)
{
    if(!IsEmpty(achievment_name) && works) {
        return ToAgsBool(works->AgsWorksCompat_ResetAchievement(achievment_name));
    }
    return 0;
}

int AgsWorksCompat_GetIntStat(const char* stat_name)
{
    if(!IsEmpty(stat_name) && works) {
        return works->AgsWorksCompat_GetIntStat(stat_name);
    }
    return 0;
}

uint32_t AgsWorksCompat_GetFloatStat(const char* stat_name)
{
    if(!IsEmpty(stat_name) && works) {
        return ToAgsFloat(works->AgsWorksCompat_GetFloatStat(stat_name));
    }
    return 0; // ags float
}

uint32_t AgsWorksCompat_GetAverageRateStat(const char* stat_name)
{
    if(!IsEmpty(stat_name) && works) {
        return ToAgsFloat(works->AgsWorksCompat_GetAverageRateStat(stat_name));
    }
    return 0; // ags float
}

int AgsWorksCompat_SetIntStat(const char* stat_name, int value)
{
    if(!IsEmpty(stat_name) && works) {
        return ToAgsBool(works->AgsWorksCompat_SetIntStat(stat_name, value));
    }
    return 0;
}

int AgsWorksCompat_SetFloatStat(const char* stat_name, uint32_t value)
{
    if(!IsEmpty(stat_name) && works) {
        return ToAgsBool(works->AgsWorksCompat_SetFloatStat(stat_name, ToNormalFloat(value)));
    }
    return 0;
}

int AgsWorksCompat_UpdateAverageRateStat(const char* stat_name, uint32_t numerator, uint32_t denominator)
{
    if(!IsEmpty(stat_name) && works) {
        return  ToAgsBool(works->AgsWorksCompat_UpdateAverageRateStat(stat_name, ToNormalFloat(numerator), ToNormalFloat(denominator)));
    }
    return 0;
}

void AgsWorksCompat_ResetAchievements()
{
    if(works) {
        works->AgsWorksCompat_ResetAchievements();
    }
}

void AgsWorksCompat_ResetStats()
{
    if(works) {
        works->AgsWorksCompat_ResetStats();
    }
}

void AgsWorksCompat_ResetStatsAndAchievements()
{
    if(works) {
        works->AgsWorksCompat_ResetStatsAndAchievements();
    }
}

int AgsWorksCompat_IsInitialized()
{
    if(works) {
        return ToAgsBool(works->AgsWorksCompat_IsInitialized());
    }
    return 0;
}

const char* AgsWorksCompat_GetCurrentLeaderboardName()
{
    if(works) {
        const char* cur_leaderboard_name = works->AgsWorksCompat_GetCurrentLeaderboardName();
        return cur_leaderboard_name != nullptr ? engine->CreateScriptString(cur_leaderboard_name) : nullptr;
    }
    return nullptr;
}

void AgsWorksCompat_RequestLeaderboard(const char *leaderboardName, int type, int limit)
{
    if(!IsEmpty(leaderboardName) && works) {
        works->AgsWorksCompat_RequestLeaderboard(leaderboardName, type, limit);
    }
}

int AgsWorksCompat_UploadScore(int score)
{
    if(works) {
        return ToAgsBool(works->AgsWorksCompat_UploadScore(score));
    }
    return 0;
}

const char* AgsWorksCompat_GetLeaderNames(int index)
{
    if(works) {
        const char* leader_name = works->AgsWorksCompat_GetLeaderNames(index);
        return leader_name != nullptr ? engine->CreateScriptString(leader_name) : nullptr;
    }
    return nullptr;
}

int AgsWorksCompat_GetLeaderScores(int index)
{
    if(works) {
        return works->AgsWorksCompat_GetLeaderScores(index);
    }
    return 0;
}

int AgsWorksCompat_GetLeaderCount()
{
    if(works) {
        return works->AgsWorksCompat_GetLeaderCount();
    }
    return 0;
}

const char* AgsWorksCompat_GetUserName()
{
    if(works) {
        const char* user_name = works->AgsWorksCompat_GetUserName();
        return user_name != nullptr ? engine->CreateScriptString(user_name) : nullptr;
    }
    return nullptr;
}

const char * AgsWorksCompat_GetCurrentGameLanguage()
{
    if(works) {
        const char* cur_language = works->AgsWorksCompat_GetCurrentGameLanguage();
        return cur_language != nullptr ? engine->CreateScriptString(cur_language) : nullptr;
    }
    return nullptr;
}

void AgsWorksCompat_FindLeaderboard(const char *leaderboardName)
{
    if(!IsEmpty(leaderboardName) && works) {
        works->AgsWorksCompat_FindLeaderboard(leaderboardName);
    }
}

// ------------------------ PLUGIN INTERFACE ------------------------------

LIBRARY_API const char *AGS_GetPluginName()
{
    #pragma EXPORT
    return "AGS Works Plugin";
}

LIBRARY_API void AGS_EngineStartup(IAGSEngine *lpEngine)
{
    #pragma EXPORT
    engine = lpEngine;

    // Make sure it's got the version with the features we need
    // 17 gives us at least CreateScriptString
    if (engine->version < 17)
    {
        engine->AbortGame ("Engine interface is too old, need newer version of AGS.");
    }
    AWLog::Init(lpEngine);

    dllAdaptor.Init();
    std::string ddl_dir = dllAdaptor.GetDllDir();
    DynamicServiceLoader::Init(ddl_dir);
    works = DynamicServiceLoader::CreateWorksDriver();
    if(works) {
        works->Init();
    }

    auto compatlist_extra = FunctionDefList();
    if(dllAdaptor.IsAnyAdapted()) {
        compatlist_extra.emplace_back("AGS2Client::IsAchievementAchieved^1", (void *)(AgsWorksCompat_IsAchievementAchieved));
        compatlist_extra.emplace_back("AGS2Client::SetAchievementAchieved^1", (void *)(AgsWorksCompat_SetAchievementAchieved));
        compatlist_extra.emplace_back("AGS2Client::ResetAchievement^1", (void *)(AgsWorksCompat_ResetAchievement));
        compatlist_extra.emplace_back("AGS2Client::GetIntStat^1", (void *)(AgsWorksCompat_GetIntStat));
        compatlist_extra.emplace_back("AGS2Client::GetFloatStat^1", (void *)(AgsWorksCompat_GetFloatStat));
        compatlist_extra.emplace_back("AGS2Client::GetAverageRateStat^1", (void *)(AgsWorksCompat_GetAverageRateStat));
        compatlist_extra.emplace_back("AGS2Client::SetIntStat^2", (void *)(AgsWorksCompat_SetIntStat));
        compatlist_extra.emplace_back("AGS2Client::SetFloatStat^2", (void *)(AgsWorksCompat_SetFloatStat));
        compatlist_extra.emplace_back("AGS2Client::UpdateAverageRateStat^3", (void *)(AgsWorksCompat_UpdateAverageRateStat));
        compatlist_extra.emplace_back("AGS2Client::ResetStatsAndAchievements^0", (void *)(AgsWorksCompat_ResetStatsAndAchievements));
        compatlist_extra.emplace_back("AGS2Client::get_Initialized", (void *)(AgsWorksCompat_IsInitialized));
        compatlist_extra.emplace_back("AGS2Client::get_CurrentLeaderboardName", (void *)(AgsWorksCompat_GetCurrentLeaderboardName));
        compatlist_extra.emplace_back("AGS2Client::RequestLeaderboard^3", (void *)(AgsWorksCompat_RequestLeaderboard));
        compatlist_extra.emplace_back("AGS2Client::UploadScore^1", (void *)(AgsWorksCompat_UploadScore));
        compatlist_extra.emplace_back("AGS2Client::geti_LeaderboardNames", (void *)(AgsWorksCompat_GetLeaderNames));
        compatlist_extra.emplace_back("AGS2Client::geti_LeaderboardScores", (void *)(AgsWorksCompat_GetLeaderScores));
        compatlist_extra.emplace_back("AGS2Client::get_LeaderboardCount", (void *)(AgsWorksCompat_GetLeaderCount));
        compatlist_extra.emplace_back("AGS2Client::GetUserName^0", (void *)(AgsWorksCompat_GetUserName));
        compatlist_extra.emplace_back("AGS2Client::GetCurrentGameLanguage^0", (void *)(AgsWorksCompat_GetCurrentGameLanguage));
        compatlist_extra.emplace_back("AGS2Client::FindLeaderboard^1", (void *)(AgsWorksCompat_FindLeaderboard));
        compatlist_extra.emplace_back("AGS2Client::Initialize^2", (void *)(AgsWorksCompat_Initialize));
        if (dllAdaptor.IsAgsSteam())
        {
            compatlist_extra.emplace_back("Steam::AddAchievement^1", (void *)(AgsWorksCompat_SetAchievementAchieved));
            compatlist_extra.emplace_back("Steam::AddStat^2", (void *)(AgsWorksCompat_SetIntStat));
            compatlist_extra.emplace_back("Steam::GetIntStat^1", (void *)(AgsWorksCompat_GetIntStat));
            compatlist_extra.emplace_back("Steam::GetFloatStat^1", (void *)(AgsWorksCompat_GetFloatStat));
            compatlist_extra.emplace_back("Steam::SetIntStat^2", (void *)(AgsWorksCompat_SetIntStat));
            compatlist_extra.emplace_back("Steam::SetFloatStat^2", (void *)(AgsWorksCompat_SetFloatStat));
            compatlist_extra.emplace_back("Steam::ResetAchievements^0", (void *)(AgsWorksCompat_ResetAchievements));
            compatlist_extra.emplace_back("Steam::ResetStats^0", (void *)(AgsWorksCompat_ResetStats));
            compatlist_extra.emplace_back("Steam::IsAchievementAchieved^1", (void *)(AgsWorksCompat_IsAchievementAchieved));
            compatlist_extra.emplace_back("Steam::SetAchievementAchieved^1", (void *)(AgsWorksCompat_SetAchievementAchieved));
            compatlist_extra.emplace_back("Steam::ResetStatsAndAchievements^0", (void *)(AgsWorksCompat_ResetStatsAndAchievements));

            compatlist_extra.emplace_back("AGSteam::IsAchievementAchieved^1", (void *)(AgsWorksCompat_IsAchievementAchieved));
            compatlist_extra.emplace_back("AGSteam::SetAchievementAchieved^1", (void *)(AgsWorksCompat_SetAchievementAchieved));
            compatlist_extra.emplace_back("AGSteam::ResetAchievement^1", (void *)(AgsWorksCompat_ResetAchievement));
            compatlist_extra.emplace_back("AGSteam::GetIntStat^1", (void *)(AgsWorksCompat_GetIntStat));
            compatlist_extra.emplace_back("AGSteam::GetFloatStat^1", (void *)(AgsWorksCompat_GetFloatStat));
            compatlist_extra.emplace_back("AGSteam::GetAverageRateStat^1", (void *)(AgsWorksCompat_GetAverageRateStat));
            compatlist_extra.emplace_back("AGSteam::SetIntStat^2", (void *)(AgsWorksCompat_SetIntStat));
            compatlist_extra.emplace_back("AGSteam::SetFloatStat^2", (void *)(AgsWorksCompat_SetFloatStat));
            compatlist_extra.emplace_back("AGSteam::UpdateAverageRateStat^3", (void *)(AgsWorksCompat_UpdateAverageRateStat));
            compatlist_extra.emplace_back("AGSteam::ResetStatsAndAchievements^0", (void *)(AgsWorksCompat_ResetStatsAndAchievements));
            compatlist_extra.emplace_back("AGSteam::get_Initialized", (void *)(AgsWorksCompat_IsInitialized));
            compatlist_extra.emplace_back("AGSteam::get_CurrentLeaderboardName", (void *)(AgsWorksCompat_GetCurrentLeaderboardName));
            compatlist_extra.emplace_back("AGSteam::RequestLeaderboard^3", (void *)(AgsWorksCompat_RequestLeaderboard));
            compatlist_extra.emplace_back("AGSteam::UploadScore^1", (void *)(AgsWorksCompat_UploadScore));
            compatlist_extra.emplace_back("AGSteam::geti_LeaderboardNames", (void *) (AgsWorksCompat_GetLeaderNames));
            compatlist_extra.emplace_back("AGSteam::geti_LeaderboardScores", (void *) (AgsWorksCompat_GetLeaderScores));
            compatlist_extra.emplace_back("AGSteam::get_LeaderboardCount", (void *)(AgsWorksCompat_GetLeaderCount));
            compatlist_extra.emplace_back("AGSteam::GetUserName^0", (void *)(AgsWorksCompat_GetUserName));
            compatlist_extra.emplace_back("AGSteam::GetCurrentGameLanguage^0", (void *)(AgsWorksCompat_GetCurrentGameLanguage));
            compatlist_extra.emplace_back("AGSteam::FindLeaderboard^1", (void *)(AgsWorksCompat_FindLeaderboard));
        }
        else if (dllAdaptor.IsAgsGalaxy())
        {
            compatlist_extra.emplace_back("AGSGalaxy::IsAchievementAchieved^1", (void *)(AgsWorksCompat_IsAchievementAchieved));
            compatlist_extra.emplace_back("AGSGalaxy::SetAchievementAchieved^1", (void *)(AgsWorksCompat_SetAchievementAchieved));
            compatlist_extra.emplace_back("AGSGalaxy::ResetAchievement^1", (void *)(AgsWorksCompat_ResetAchievement));
            compatlist_extra.emplace_back("AGSGalaxy::GetIntStat^1", (void *)(AgsWorksCompat_GetIntStat));
            compatlist_extra.emplace_back("AGSGalaxy::GetFloatStat^1", (void *)(AgsWorksCompat_GetFloatStat));
            compatlist_extra.emplace_back("AGSGalaxy::GetAverageRateStat^1", (void *)(AgsWorksCompat_GetAverageRateStat));
            compatlist_extra.emplace_back("AGSGalaxy::SetIntStat^2", (void *)(AgsWorksCompat_SetIntStat));
            compatlist_extra.emplace_back("AGSGalaxy::SetFloatStat^2", (void *)(AgsWorksCompat_SetFloatStat));
            compatlist_extra.emplace_back("AGSGalaxy::UpdateAverageRateStat^3", (void *)(AgsWorksCompat_UpdateAverageRateStat));
            compatlist_extra.emplace_back("AGSGalaxy::ResetStatsAndAchievements^0", (void *)(AgsWorksCompat_ResetStatsAndAchievements));
            compatlist_extra.emplace_back("AGSGalaxy::get_Initialized", (void *)(AgsWorksCompat_IsInitialized));
            compatlist_extra.emplace_back("AGSGalaxy::get_CurrentLeaderboardName", (void *)(AgsWorksCompat_GetCurrentLeaderboardName));
            compatlist_extra.emplace_back("AGSGalaxy::RequestLeaderboard^3", (void *)(AgsWorksCompat_RequestLeaderboard));
            compatlist_extra.emplace_back("AGSGalaxy::UploadScore^1", (void *)(AgsWorksCompat_UploadScore));
            compatlist_extra.emplace_back("AGSGalaxy::geti_LeaderboardNames", (void *) (AgsWorksCompat_GetLeaderNames));
            compatlist_extra.emplace_back("AGSGalaxy::geti_LeaderboardScores", (void *) (AgsWorksCompat_GetLeaderScores));
            compatlist_extra.emplace_back("AGSGalaxy::get_LeaderboardCount", (void *)(AgsWorksCompat_GetLeaderCount));
            compatlist_extra.emplace_back("AGSGalaxy::GetUserName^0", (void *)(AgsWorksCompat_GetUserName));
            compatlist_extra.emplace_back("AGSGalaxy::GetCurrentGameLanguage^0", (void *)(AgsWorksCompat_GetCurrentGameLanguage));
            compatlist_extra.emplace_back("AGSGalaxy::Initialize^2", (void *)(AgsWorksCompat_Initialize));
        }
    }

    FunctionDefList compatlist {{"AgsWorksCompat::IsAchievementAchieved^1", (void *)(AgsWorksCompat_IsAchievementAchieved)},
            {"AgsWorksCompat::SetAchievementAchieved^1", (void *)(AgsWorksCompat_SetAchievementAchieved)},
            {"AgsWorksCompat::ResetAchievement^1", (void *)(AgsWorksCompat_ResetAchievement)},
            {"AgsWorksCompat::GetIntStat^1", (void *)(AgsWorksCompat_GetIntStat)},
            {"AgsWorksCompat::GetFloatStat^1", (void *)(AgsWorksCompat_GetFloatStat)},
            {"AgsWorksCompat::GetAverageRateStat^1", (void *)(AgsWorksCompat_GetAverageRateStat)},
            {"AgsWorksCompat::SetIntStat^2", (void *)(AgsWorksCompat_SetIntStat)},
            {"AgsWorksCompat::SetFloatStat^2", (void *)(AgsWorksCompat_SetFloatStat)},
            {"AgsWorksCompat::UpdateAverageRateStat^3", (void *)(AgsWorksCompat_UpdateAverageRateStat)},
            {"AgsWorksCompat::ResetStatsAndAchievements^0", (void *)(AgsWorksCompat_ResetStatsAndAchievements)},
            {"AgsWorksCompat::get_Initialized", (void *)(AgsWorksCompat_IsInitialized)},
            {"AgsWorksCompat::Initialize^2", (void *)(AgsWorksCompat_Initialize)},
            {"AgsWorksCompat::get_CurrentLeaderboardName", (void *)(AgsWorksCompat_GetCurrentLeaderboardName)},
            {"AgsWorksCompat::RequestLeaderboard^3", (void *)(AgsWorksCompat_RequestLeaderboard)},
            {"AgsWorksCompat::UploadScore^1", (void *)(AgsWorksCompat_UploadScore)},
            {"AgsWorksCompat::geti_LeaderboardNames", (void *)(AgsWorksCompat_GetLeaderNames)},
            {"AgsWorksCompat::geti_LeaderboardScores", (void *)(AgsWorksCompat_GetLeaderScores)},
            {"AgsWorksCompat::get_LeaderboardCount", (void *)(AgsWorksCompat_GetLeaderCount)},
            {"AgsWorksCompat::GetUserName^0", (void *)(AgsWorksCompat_GetUserName)},
            {"AgsWorksCompat::GetCurrentGameLanguage^0", (void *)(AgsWorksCompat_GetCurrentGameLanguage)},
    };

    for(const auto& f : compatlist)
    {
        engine->RegisterScriptFunction(f.first.c_str(), f.second);
    }
    for(const auto& f : compatlist_extra)
    {
        engine->RegisterScriptFunction(f.first.c_str(), f.second);
    }

    engine->RequestEventHook(AGSE_FINALSCREENDRAW);
}

LIBRARY_API void AGS_EngineShutdown()
{
    #pragma EXPORT
    if(works){
        works->Shutdown();
        delete works;
        works = nullptr;
    }
    DynamicServiceLoader::Shutdown();
    AWLog::Shutdown();
}

LIBRARY_API int AGS_EngineOnEvent (int event, int data)
{
    #pragma EXPORT
    if (event == AGSE_FINALSCREENDRAW)
    {
        // use this as main_game_loop
        // the screen is drawn every actual game loop anyway
        if(works) {
            works->Update();
        }
    }
    return 0;
}

LIBRARY_API int AGS_EngineDebugHook(const char* scriptName, int lineNum, int reserved)
{
    #pragma EXPORT
    return 0;
}

LIBRARY_API void AGS_EngineInitGfx(const char* driverID, void* data)
{
    #pragma EXPORT
    // nothing
}

// ****** END RUN TIME ********
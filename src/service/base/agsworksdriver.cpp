
#include "service/base/agsworksdriver.h"

namespace AGSWorks
{


ServiceType AGSWorksDriver::GetServiceType() {
    return ServiceType::eNone;
}

void AGSWorksDriver::Init() {

}

void AGSWorksDriver::Shutdown() {
    // do nothing
}

void AGSWorksDriver::AgsWorksCompat_Initialize(const char *clientID, const char *clientSecret) {
    // do nothing
}

bool AGSWorksDriver::AgsWorksCompat_IsAchievementAchieved(const char *achievment_name) {
    return false;
}

bool AGSWorksDriver::AgsWorksCompat_SetAchievementAchieved(const char *achievment_name) {
    return false;
}

bool AGSWorksDriver::AgsWorksCompat_ResetAchievement(const char *achievment_name) {
    return false;
}

int AGSWorksDriver::AgsWorksCompat_GetIntStat(const char *stat_name) {
    return 0;
}

float AGSWorksDriver::AgsWorksCompat_GetFloatStat(const char *stat_name) {
    return 0;
}

float AGSWorksDriver::AgsWorksCompat_GetAverageRateStat(const char *stat_name) {
    return 0;
}

bool AGSWorksDriver::AgsWorksCompat_SetIntStat(const char *stat_name, int value) {
    return false;
}

bool AGSWorksDriver::AgsWorksCompat_SetFloatStat(const char *stat_name, float value) {
    return false;
}

bool AGSWorksDriver::AgsWorksCompat_UpdateAverageRateStat(const char *stat_name, float numerator, float denominator) {
    return false;
}

void AGSWorksDriver::AgsWorksCompat_ResetAchievements() {
    // do nothing
}

void AGSWorksDriver::AgsWorksCompat_ResetStats() {
    // do nothing
}

void AGSWorksDriver::AgsWorksCompat_ResetStatsAndAchievements() {
    // do nothing
}

bool AGSWorksDriver::AgsWorksCompat_IsInitialized() {
    return false;
}

const char *AGSWorksDriver::AgsWorksCompat_GetCurrentLeaderboardName() {
    return nullptr;
}

void AGSWorksDriver::AgsWorksCompat_RequestLeaderboard(const char *leaderboardName, int type, int limit) {
    // do nothing
}

bool AGSWorksDriver::AgsWorksCompat_UploadScore(int score) {
    return false;
}

const char *AGSWorksDriver::AgsWorksCompat_GetLeaderNames(int index) {
    return nullptr;
}

int AGSWorksDriver::AgsWorksCompat_GetLeaderScores(int index) {
    return 0;
}

int AGSWorksDriver::AgsWorksCompat_GetLeaderCount() {
    return 0;
}

const char * AGSWorksDriver::AgsWorksCompat_GetUserName() {
    return nullptr;
}

const char * AGSWorksDriver::AgsWorksCompat_GetCurrentGameLanguage() {
    return nullptr;
}

void AGSWorksDriver::AgsWorksCompat_FindLeaderboard(const char *leaderboardName) {
    // do nothing
}

} // namespace AGSWorks
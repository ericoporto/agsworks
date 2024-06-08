//
// Created by erico on 6/7/2024.
//

#ifndef AGSWORKS_DYNAPI_STEAM_H
#define AGSWORKS_DYNAPI_STEAM_H

#include <cstdint>

namespace AGSWorks {
    // handle to a communication pipe to the Steam client
    typedef int32_t HSteamPipe;
    // handle to single instance of a steam user
    typedef int32_t HSteamUser;

    enum { k_iSteamUserStatsCallbacks = 1100 };

    enum EResult
    {
        k_EResultNone = 0,                            // no result
        k_EResultOK    = 1,                            // success
        k_EResultFail = 2,                            // generic failure
        k_EResultNoConnection = 3,                    // no/failed network connection
        k_EResultInvalidPassword = 5,                // password/ticket is invalid
        k_EResultLoggedInElsewhere = 6,                // same user logged in elsewhere
        k_EResultInvalidProtocolVer = 7,            // protocol version is incorrect
        k_EResultInvalidParam = 8,                    // a parameter is incorrect
        k_EResultFileNotFound = 9,                    // file was not found
        k_EResultBusy = 10,                            // called method busy - action not taken
        k_EResultInvalidState = 11,                    // called object was in an invalid state
        k_EResultInvalidName = 12,                    // name is invalid
        k_EResultInvalidEmail = 13,                    // email is invalid
        k_EResultDuplicateName = 14,                // name is not unique
        k_EResultAccessDenied = 15,                    // access is denied
        k_EResultTimeout = 16,                        // operation timed out
        k_EResultBanned = 17,                        // VAC2 banned
        k_EResultAccountNotFound = 18,                // account not found
        k_EResultInvalidSteamID = 19,                // steamID is invalid
        k_EResultServiceUnavailable = 20,            // The requested service is currently unavailable
        k_EResultNotLoggedOn = 21,                    // The user is not logged on
        k_EResultPending = 22,                        // Request is pending (may be in process, or waiting on third party)
        k_EResultEncryptionFailure = 23,            // Encryption or Decryption failed
        k_EResultInsufficientPrivilege = 24,        // Insufficient privilege
        k_EResultLimitExceeded = 25,                // Too much of a good thing
        k_EResultRevoked = 26,                        // Access has been revoked (used for revoked guest passes)
        k_EResultExpired = 27,                        // License/Guest pass the user is trying to access is expired
        k_EResultAlreadyRedeemed = 28,                // Guest pass has already been redeemed by account, cannot be acked again
        k_EResultDuplicateRequest = 29,                // The request is a duplicate and the action has already occurred in the past, ignored this time
        k_EResultAlreadyOwned = 30,                    // All the games in this guest pass redemption request are already owned by the user
        k_EResultIPNotFound = 31,                    // IP address not found
        k_EResultPersistFailed = 32,                // failed to write change to the data store
        k_EResultLockingFailed = 33,                // failed to acquire access lock for this operation
        k_EResultLogonSessionReplaced = 34,
        k_EResultConnectFailed = 35,
        k_EResultHandshakeFailed = 36,
        k_EResultIOFailure = 37,
        k_EResultRemoteDisconnect = 38,
        k_EResultShoppingCartNotFound = 39,            // failed to find the shopping cart requested
        k_EResultBlocked = 40,                        // a user didn't allow it
        k_EResultIgnored = 41,                        // target is ignoring sender
        k_EResultNoMatch = 42,                        // nothing matching the request found
        k_EResultAccountDisabled = 43,
        k_EResultServiceReadOnly = 44,                // this service is not accepting content changes right now
        k_EResultAccountNotFeatured = 45,            // account doesn't have value, so this feature isn't available
        k_EResultAdministratorOK = 46,                // allowed to take this action, but only because requester is admin
        k_EResultContentVersion = 47,                // A Version mismatch in content transmitted within the Steam protocol.
        k_EResultTryAnotherCM = 48,                    // The current CM can't service the user making a request, user should try another.
        k_EResultPasswordRequiredToKickSession = 49,// You are already logged in elsewhere, this cached credential login has failed.
        k_EResultAlreadyLoggedInElsewhere = 50,        // You are already logged in elsewhere, you must wait
        k_EResultSuspended = 51,                    // Long running operation (content download) suspended/paused
        k_EResultCancelled = 52,                    // Operation canceled (typically by user: content download)
        k_EResultDataCorruption = 53,                // Operation canceled because data is ill formed or unrecoverable
        k_EResultDiskFull = 54,                        // Operation canceled - not enough disk space.
        k_EResultRemoteCallFailed = 55,                // an remote call or IPC call failed
        k_EResultPasswordUnset = 56,                // Password could not be verified as it's unset server side
        k_EResultExternalAccountUnlinked = 57,        // External account (PSN, Facebook...) is not linked to a Steam account
        k_EResultPSNTicketInvalid = 58,                // PSN ticket was invalid
        k_EResultExternalAccountAlreadyLinked = 59,    // External account (PSN, Facebook...) is already linked to some other account, must explicitly request to replace/delete the link first
        k_EResultRemoteFileConflict = 60,            // The sync cannot resume due to a conflict between the local and remote files
        k_EResultIllegalPassword = 61,                // The requested new password is not legal
        k_EResultSameAsPreviousValue = 62,            // new value is the same as the old one ( secret question and answer )
        k_EResultAccountLogonDenied = 63,            // account login denied due to 2nd factor authentication failure
        k_EResultCannotUseOldPassword = 64,            // The requested new password is not legal
        k_EResultInvalidLoginAuthCode = 65,            // account login denied due to auth code invalid
        k_EResultAccountLogonDeniedNoMail = 66,        // account login denied due to 2nd factor auth failure - and no mail has been sent - partner site specific
        k_EResultHardwareNotCapableOfIPT = 67,        //
        k_EResultIPTInitError = 68,                    //
        k_EResultParentalControlRestricted = 69,    // operation failed due to parental control restrictions for current user
        k_EResultFacebookQueryError = 70,            // Facebook query returned an error
        k_EResultExpiredLoginAuthCode = 71,            // account login denied due to auth code expired
        k_EResultIPLoginRestrictionFailed = 72,
        k_EResultAccountLockedDown = 73,
        k_EResultAccountLogonDeniedVerifiedEmailRequired = 74,
        k_EResultNoMatchingURL = 75,
        k_EResultBadResponse = 76,                    // parse failure, missing field, etc.
        k_EResultRequirePasswordReEntry = 77,        // The user cannot complete the action until they re-enter their password
        k_EResultValueOutOfRange = 78,                // the value entered is outside the acceptable range
        k_EResultUnexpectedError = 79,                // something happened that we didn't expect to ever happen
        k_EResultDisabled = 80,                        // The requested service has been configured to be unavailable
        k_EResultInvalidCEGSubmission = 81,            // The set of files submitted to the CEG server are not valid !
        k_EResultRestrictedDevice = 82,                // The device being used is not allowed to perform this action
        k_EResultRegionLocked = 83,                    // The action could not be complete because it is region restricted
        k_EResultRateLimitExceeded = 84,            // Temporary rate limit exceeded, try again later, different from k_EResultLimitExceeded which may be permanent
        k_EResultAccountLoginDeniedNeedTwoFactor = 85,    // Need two-factor code to login
        k_EResultItemDeleted = 86,                    // The thing we're trying to access has been deleted
        k_EResultAccountLoginDeniedThrottle = 87,    // login attempt failed, try to throttle response to possible attacker
        k_EResultTwoFactorCodeMismatch = 88,        // two factor code mismatch
        k_EResultTwoFactorActivationCodeMismatch = 89,    // activation code for two-factor didn't match
        k_EResultAccountAssociatedToMultiplePartners = 90,    // account has been associated with multiple partners
        k_EResultNotModified = 91,                    // data not modified
        k_EResultNoMobileDevice = 92,                // the account does not have a mobile device associated with it
        k_EResultTimeNotSynced = 93,                // the time presented is out of range or tolerance
        k_EResultSmsCodeFailed = 94,                // SMS code failure (no match, none pending, etc.)
        k_EResultAccountLimitExceeded = 95,            // Too many accounts access this resource
        k_EResultAccountActivityLimitExceeded = 96,    // Too many changes to this account
        k_EResultPhoneActivityLimitExceeded = 97,    // Too many changes to this phone
        k_EResultRefundToWallet = 98,                // Cannot refund to payment method, must use wallet
        k_EResultEmailSendFailure = 99,                // Cannot send an email
        k_EResultNotSettled = 100,                    // Can't perform operation till payment has settled
        k_EResultNeedCaptcha = 101,                    // Needs to provide a valid captcha
        k_EResultGSLTDenied = 102,                    // a game server login token owned by this token's owner has been banned
        k_EResultGSOwnerDenied = 103,                // game server owner is denied for other reason (account lock, community ban, vac ban, missing phone)
        k_EResultInvalidItemType = 104,                // the type of thing we were requested to act on is invalid
        k_EResultIPBanned = 105,                    // the ip address has been banned from taking this action
        k_EResultGSLTExpired = 106,                    // this token has expired from disuse; can be reset for use
        k_EResultInsufficientFunds = 107,            // user doesn't have enough wallet funds to complete the action
        k_EResultTooManyPending = 108,                // There are too many of this thing pending already
        k_EResultNoSiteLicensesFound = 109,            // No site licenses found
        k_EResultWGNetworkSendExceeded = 110,        // the WG couldn't send a response because we exceeded max network send size
        k_EResultAccountNotFriends = 111,            // the user is not mutually friends
        k_EResultLimitedUserAccount = 112,            // the user is limited
        k_EResultCantRemoveItem = 113,                // item can't be removed
        k_EResultAccountDeleted = 114,                // account has been deleted
        k_EResultExistingUserCancelledLicense = 115,    // A license for this already exists, but cancelled
        k_EResultCommunityCooldown = 116,            // access is denied because of a community cooldown (probably from support profile data resets)
        k_EResultNoLauncherSpecified = 117,            // No launcher was specified, but a launcher was needed to choose correct realm for operation.
        k_EResultMustAgreeToSSA = 118,                // User must agree to china SSA or global SSA before login
        k_EResultLauncherMigrated = 119,            // The specified launcher type is no longer supported; the user should be directed elsewhere
        k_EResultSteamRealmMismatch = 120,            // The user's realm does not match the realm of the requested resource
        k_EResultInvalidSignature = 121,            // signature check did not match
        k_EResultParseFailure = 122,                // Failed to parse input
        k_EResultNoVerifiedPhone = 123,                // account does not have a verified phone number
        k_EResultInsufficientBattery = 124,            // user device doesn't have enough battery charge currently to complete the action
        k_EResultChargerRequired = 125,                // The operation requires a charger to be plugged in, which wasn't present
        k_EResultCachedCredentialInvalid = 126,        // Cached credential was invalid - user must reauthenticate
        K_EResultPhoneNumberIsVOIP = 127,            // The phone number provided is a Voice Over IP number
        k_EResultNotSupported = 128,                // The data being accessed is not supported by this API
        k_EResultFamilySizeLimitExceeded = 129,        // Reached the maximum size of the family
    };

    enum EUniverse
    {
        k_EUniverseInvalid = 0,
        k_EUniversePublic = 1,
        k_EUniverseBeta = 2,
        k_EUniverseInternal = 3,
        k_EUniverseDev = 4,
        k_EUniverseMax
    };

    struct ISteamClient;
    struct ISteamScreenshots;
    struct ISteamUserStats;

    struct CallbackMsg_t {
        HSteamUser m_hSteamUser;
        int32_t m_iCallback;
        uint8_t *m_pubParam;
        int32_t m_cubParam;
    };

    class CSteamID
    {

    private:
        // These are defined here to prevent accidental implicit conversion of a u32AccountID to a CSteamID.
        // If you get a compiler error about an ambiguous constructor/function then it may be because you're
        // passing a 32-bit int to a function that takes a CSteamID. You should explicitly create the SteamID
        // using the correct Universe and account Type/Instance values.
        CSteamID( uint32_t );
        CSteamID( int32_t );

        // 64 bits total
        union SteamID_t
        {
            struct SteamIDComponent_t
            {
#if AGS_PLATFORM_ENDIAN_BIG
                EUniverse    m_EUniverse : 8;          // universe this account belongs to
                unsigned int m_EAccountType : 4;       // type of account - can't show as EAccountType, due to signed / unsigned difference
                unsigned int m_unAccountInstance : 20; // dynamic instance ID
                uint32       m_unAccountID : 32;       // unique account identifier
#else
                uint32_t     m_unAccountID : 32;       // unique account identifier
                unsigned int m_unAccountInstance : 20; // dynamic instance ID
                unsigned int m_EAccountType : 4;       // type of account - can't show as EAccountType, due to signed / unsigned difference
                EUniverse    m_EUniverse : 8;          // universe this account belongs to
#endif
            } m_comp;

            uint64_t m_unAll64Bits;
        } m_steamid;
    };

    struct UserStatsReceived_t
    {
        enum { k_iCallback = k_iSteamUserStatsCallbacks + 1 };
        uint64_t        m_nGameID;        // Game these stats are for
        EResult        m_eResult;        // Success / error fetching the stats
        CSteamID    m_steamIDUser;    // The user for whom the stats are retrieved for
    };
    struct SteamAPICallCompleted_t {
        uint64_t m_hAsyncCall;
        int32_t m_iCallback;
        uint32_t m_cubParam;
    };


    const int k_cchMaxSteamErrMsg = 1024;
    typedef char SteamErrMsg[ k_cchMaxSteamErrMsg ];

    enum ESteamAPIInitResult
    {
        k_ESteamAPIInitResult_OK = 0,
        k_ESteamAPIInitResult_FailedGeneric = 1, // Some other failure
        k_ESteamAPIInitResult_NoSteamClient = 2, // We cannot connect to Steam, steam probably isn't running
        k_ESteamAPIInitResult_VersionMismatch = 3, // Steam client appears to be out of date
    };

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
        using C_STEAMAPI_MANUALDISPATCH_GETAPICALLRESULT =  uint8_t (*)(int32_t, uint64_t, void *, int32_t, int32_t, uint8_t *);
        C_STEAMAPI_MANUALDISPATCH_GETAPICALLRESULT c_SteamAPI_ManualDispatch_GetAPICallResult;

    public:

        const char * STEAMCLIENT_INTERFACE_VERSION = "SteamClient021";
        const char * STEAMUSERSTATS_INTERFACE_VERSION = "STEAMUSERSTATS_INTERFACE_VERSION012";

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
    };

}

#endif //AGSWORKS_DYNAPI_STEAM_H

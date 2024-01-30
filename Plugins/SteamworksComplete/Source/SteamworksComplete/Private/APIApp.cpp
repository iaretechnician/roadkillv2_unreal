/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "APIApp.h"
#include "SteamworksComplete.h"

UAPIApp::UAPIApp()
{
    // Constructor code here
}

bool UAPIApp::IsSteamInitialized()
{
#if UE_SERVER
    if (SteamGameServer() == nullptr)
        return false;
#else
    if (SteamClient() == nullptr)
        return false;
#endif
    else
        return true;
}

int32 UAPIApp::SteamGetAppId()
{
    uint32 id = SteamUtils()->GetAppID();
    return static_cast<int32>(id);
}

void UAPIApp::SteamShutdown()
{
#if UE_SERVER
    SteamGameServer_Shutdown();
#else
    SteamAPI_Shutdown();
#endif
}

int32 UAPIApp::SteamGetAppIdFromGameId(int64 gameId)
{
    CGameID game = CGameID(static_cast<uint64>(gameId));
    return static_cast<int32>(game.AppID());
}

int64 UAPIApp::SteamGetGameIdFromAppId(int32 appId)
{
    CGameID game = CGameID(appId);
    return static_cast<int64>(game.m_ulGameID);
}

int64 UAPIApp::SteamGetSteamIdFromAccountId(int32 accountId, UEAccountType type)
{
    CSteamID steamId = CSteamID(static_cast<uint32>(accountId), EUniverse::k_EUniversePublic, static_cast<EAccountType>(type));
    return STEAMID_TO_INT64(steamId);
}

int32 UAPIApp::SteamGetAccountIdFromSteamId(int64 steamId)
{
    CSteamID steam = INT64_TO_STEAMID(steamId);
    return static_cast<int32>(steam.GetAccountID());
}

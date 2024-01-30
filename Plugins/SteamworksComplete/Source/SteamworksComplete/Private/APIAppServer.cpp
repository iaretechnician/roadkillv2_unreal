/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "APIAppServer.h"

void UAPIAppServer::SteamServerAssociateWithClan(int64 clanId, FAssociateWithClanResult callback)
{
	STEAM_SERVER(;);
	FAssociateWithClanResultLinker* linker = new FAssociateWithClanResultLinker(clanId, callback);
}

UEBeginAuthSessionResult UAPIAppServer::SteamServerBeginAuthSession(TArray<uint8> authTicket, int64 steamId)
{
	STEAM_SERVER(UEBeginAuthSessionResult::EPC_InvalidTicket);

	// Convert the FString AuthTicket to a byte array (uint8)
	TArray<uint8> TicketData;
	int32 TicketLength = authTicket.Num() * sizeof(uint8); // Get the length dynamically

	TicketData.SetNumUninitialized(TicketLength);
	FMemory::Memcpy(TicketData.GetData(), authTicket.GetData(), TicketLength);

	// Call the actual BeginAuthSession function with the appropriate arguments
	EBeginAuthSessionResult result = SteamGameServer()->BeginAuthSession(TicketData.GetData(), TicketLength, INT64_TO_STEAMID(steamId));

	return static_cast<UEBeginAuthSessionResult>(result);
}

bool UAPIAppServer::SteamServerLoggedOn()
{
	STEAM_SERVER(false);
	return SteamGameServer()->BLoggedOn();
}

bool UAPIAppServer::SteamServerSecure()
{
	STEAM_SERVER(false);
	return SteamGameServer()->BSecure();
}

void UAPIAppServer::SteamServerCancelAuthTicket(int32 authTicketHandle)
{
	STEAM_SERVER(;);
	SteamGameServer()->CancelAuthTicket(static_cast<HAuthTicket>(authTicketHandle));
}

void UAPIAppServer::SteamServerClearAllKeyValues()
{
	STEAM_SERVER(;);
	SteamGameServer()->ClearAllKeyValues();
}

void UAPIAppServer::SteamServerComputeNewPlayerCompatibility(int64 userId, FComputeNewPlayerCompatibilityResult callback)
{
	STEAM_SERVER(;);
	FComputeNewPlayerCompatibilityResultLinker* linker = new FComputeNewPlayerCompatibilityResultLinker(userId, callback);
}

void UAPIAppServer::SteamServerSetAdvertiseServerActive(bool active)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetAdvertiseServerActive(active);
}

void UAPIAppServer::SteamServerEndAuthSession(int64 steamId)
{
	STEAM_SERVER(;);
	SteamGameServer()->EndAuthSession(INT64_TO_STEAMID(steamId));
}

FAuthTicketData UAPIAppServer::SteamServerGetAuthSessionTicket(int64 forSteamId)
{
	STEAM_SERVER(FAuthTicketData());

	TArray<uint8> buffer;
	buffer.SetNumUninitialized(1024);
	uint32 BytesWritten = 0;
	SteamNetworkingIdentity Id{};
	Id.m_eType = ESteamNetworkingIdentityType::k_ESteamNetworkingIdentityType_SteamID;
	Id.SetSteamID(INT64_TO_STEAMID(forSteamId));
	HAuthTicket handle = SteamGameServer()->GetAuthSessionTicket(buffer.GetData(), 1024, &BytesWritten, &Id);

	FAuthTicketData result;
	result.authTicketHandle = static_cast<int32>(handle);
	result.Ticket = buffer;

	return result;
}

/// <summary>
/// Currently only working with IPv4
/// </summary>
/// <returns></returns>
FString UAPIAppServer::SteamServerGetPublicIP()
{
	STEAM_SERVER(FString());
	SteamIPAddress_t address = SteamGameServer()->GetPublicIP();	
	return HeathenTools::IPAddress(address.m_unIPv4);
}

int64 UAPIAppServer::SteamServerGetSteamID()
{
	STEAM_SERVER(0);
	return STEAMID_TO_INT64(SteamGameServer()->GetSteamID());
}

bool UAPIAppServer::SteamServerInitialize(FString ip, int32 gamePort, int32 queryPort, UEServerMode mode, FString version)
{
	uint32 uIp = HeathenTools::IPAddress(ip);
	uint16 gPort = static_cast<uint16>(gamePort);
	uint16 qPort = static_cast<uint16>(queryPort);
	return SteamGameServer_Init(uIp, gPort, qPort, static_cast<EServerMode>(mode), StringCast<ANSICHAR>(*version).Get());
}

void UAPIAppServer::SteamServerRunCallbacks()
{
	STEAM_SERVER(;);
	SteamGameServer_RunCallbacks();
}

void UAPIAppServer::SteamServerSetServerSettings(FString name, FString description, FString product, FString map, FString modDirectory, bool passwordProtected, bool isDedicated, int maxPlayers, int maxBots, FString gameData, FString gameTags, TMap<FString, FString> keyValues)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetServerName(StringCast<ANSICHAR>(*name).Get());
	SteamGameServer()->SetGameDescription(StringCast<ANSICHAR>(*description).Get());
	SteamGameServer()->SetProduct(StringCast<ANSICHAR>(*product).Get());
	SteamGameServer()->SetMapName(StringCast<ANSICHAR>(*map).Get());
	SteamGameServer()->SetModDir(StringCast<ANSICHAR>(*modDirectory).Get());
	SteamGameServer()->SetPasswordProtected(passwordProtected);
	SteamGameServer()->SetDedicatedServer(isDedicated);
	SteamGameServer()->SetMaxPlayerCount(maxPlayers);
	SteamGameServer()->SetBotPlayerCount(maxBots);
	if (!gameData.IsEmpty())
		SteamGameServer()->SetGameData(StringCast<ANSICHAR>(*gameData).Get());
	if (!gameTags.IsEmpty())
		SteamGameServer()->SetGameTags(StringCast<ANSICHAR>(*gameTags).Get());
	if (keyValues.Num() > 0)
	{
		for (auto& Elem : keyValues)
		{
			SteamGameServer()->SetKeyValue(StringCast<ANSICHAR>(*Elem.Key).Get(), StringCast<ANSICHAR>(*Elem.Value).Get());
		}
	}
}

void UAPIAppServer::SteamServerLogOff()
{
	STEAM_SERVER(;);
	SteamGameServer()->LogOff();
}

void UAPIAppServer::SteamServerLogOn(FString token)
{
	STEAM_SERVER(;);
	SteamGameServer()->LogOn(StringCast<ANSICHAR>(*token).Get());
}

void UAPIAppServer::SteamServerLogOnAnonymous()
{
	STEAM_SERVER(;);
	SteamGameServer()->LogOnAnonymous();
}

bool UAPIAppServer::SteamServerRequestUserGroupStatus(int64 userId, int64 groupId)
{
	STEAM_SERVER(false);
	return SteamGameServer()->RequestUserGroupStatus(INT64_TO_STEAMID(userId), INT64_TO_STEAMID(groupId));
}

void UAPIAppServer::SteamServerSetBotPlayerCount(int botPlayers)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetBotPlayerCount(botPlayers);
}

void UAPIAppServer::SteamServerSetDedicatedServer(bool dedicated)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetDedicatedServer(dedicated);
}

void UAPIAppServer::SteamServerSetGameData(FString data)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetGameData(StringCast<ANSICHAR>(*data).Get());
}

void UAPIAppServer::SteamServerSetDescription(FString description)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetGameDescription(StringCast<ANSICHAR>(*description).Get());
}

void UAPIAppServer::SteamServerSetGameTags(FString tags)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetGameTags(StringCast<ANSICHAR>(*tags).Get());
}

void UAPIAppServer::SteamServerSetKeyValue(FString key, FString value)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetKeyValue(StringCast<ANSICHAR>(*key).Get(), StringCast<ANSICHAR>(*value).Get());
}

void UAPIAppServer::SteamServerSetMapName(FString name)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetMapName(StringCast<ANSICHAR>(*name).Get());
}

void UAPIAppServer::SteamServerSetMaxPlayerCount(int32 playersMax)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetMaxPlayerCount(playersMax);
}

void UAPIAppServer::SteamServerSetModDirectoryName(FString directoryName)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetModDir(StringCast<ANSICHAR>(*directoryName).Get());
}

void UAPIAppServer::SteamServerSetPasswordProtected(bool isPasswordProtected)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetPasswordProtected(isPasswordProtected);
}

void UAPIAppServer::SteamServerSetProduct(FString product)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetProduct(StringCast<ANSICHAR>(*product).Get());
}

void UAPIAppServer::SteamServerSetRegion(FString region)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetRegion(StringCast<ANSICHAR>(*region).Get());
}

void UAPIAppServer::SteamServerSetName(FString name)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetServerName(StringCast<ANSICHAR>(*name).Get());
}

void UAPIAppServer::SteamServerSetSpectatorPort(int32 port)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetSpectatorPort(static_cast<uint16>(port));
}

void UAPIAppServer::SteamServerSetSpectatorName(FString name)
{
	STEAM_SERVER(;);
	SteamGameServer()->SetSpectatorServerName(StringCast<ANSICHAR>(*name).Get());
}

UEUserHasLicenseForAppResult UAPIAppServer::SteamServerUserHasLicenseForApp(int64 user, int32 app)
{
	STEAM_SERVER(UEUserHasLicenseForAppResult::EPC_NoAuth);
	EUserHasLicenseForAppResult result = SteamGameServer()->UserHasLicenseForApp(INT64_TO_STEAMID(user), static_cast<AppId_t>(app));
	return static_cast<UEUserHasLicenseForAppResult>(result);
}

bool UAPIAppServer::SteamServerWasRestartRequested()
{
	STEAM_SERVER(false);
	return SteamGameServer()->WasRestartRequested();
}

bool UAPIAppServer::SteamServerClearUserAchievement(int64 user, FString apiName)
{
	STEAM_SERVER(false);
	return SteamGameServerStats()->ClearUserAchievement(INT64_TO_STEAMID(user), StringCast<ANSICHAR>(*apiName).Get());
}

FServerAchievementStatus UAPIAppServer::SteamServerGetUserAchievement(int64 user, FString apiName)
{
	FServerAchievementStatus result = FServerAchievementStatus();
	STEAM_SERVER(result);
	bool achieved;
	result.Success = SteamGameServerStats()->GetUserAchievement(INT64_TO_STEAMID(user), StringCast<ANSICHAR>(*apiName).Get(), &achieved);
	result.Achieved = achieved;
	return result;
}

FServerStatIntValue UAPIAppServer::SteamServerGetUserIntStat(int64 user, FString apiName)
{
	FServerStatIntValue result = FServerStatIntValue();
	STEAM_SERVER(result);
	int32 value;
	result.Success = SteamGameServerStats()->GetUserStat(INT64_TO_STEAMID(user), StringCast<ANSICHAR>(*apiName).Get(), &value);
	result.Value = value;
	return result;
}

FServerStatFloatValue UAPIAppServer::SteamServerGetUserFloatStat(int64 user, FString apiName)
{
	FServerStatFloatValue result = FServerStatFloatValue();
	STEAM_SERVER(result);
	float value;
	result.Success = SteamGameServerStats()->GetUserStat(INT64_TO_STEAMID(user), StringCast<ANSICHAR>(*apiName).Get(), &value);
	result.Value = value;
	return result;
}

void UAPIAppServer::SteamServerRequestUserStats(int64 user, FGSStatsCallback callback)
{
	FGSStatsReceivedLinker* linker = new FGSStatsReceivedLinker(user, callback);
}

bool UAPIAppServer::SteamServerSetUserAchievement(int64 user, FString apiName)
{
	STEAM_SERVER(false);
	return SteamGameServerStats()->SetUserAchievement(INT64_TO_STEAMID(user), StringCast<ANSICHAR>(*apiName).Get());
}

bool UAPIAppServer::SteamServerSetUserIntStat(int64 user, FString apiName, int32 value)
{
	STEAM_SERVER(false);
	return SteamGameServerStats()->SetUserStat(INT64_TO_STEAMID(user), StringCast<ANSICHAR>(*apiName).Get(), value);
}

bool UAPIAppServer::SteamServerSetUserFloatStat(int64 user, FString apiName, float value)
{
	STEAM_SERVER(false);
	return SteamGameServerStats()->SetUserStat(INT64_TO_STEAMID(user), StringCast<ANSICHAR>(*apiName).Get(), value);
}

void UAPIAppServer::SteamServerStoreUserStats(int64 user, FGSStatsCallback callback)
{
	STEAM_SERVER(;);
	SteamGameServerStats()->StoreUserStats(INT64_TO_STEAMID(user));
}

bool UAPIAppServer::SteamServerUpdateUserAvgRateStat(int64 user, FString apiName, float value, double length)
{
	STEAM_SERVER(false);
	return SteamGameServerStats()->UpdateUserAvgRateStat(INT64_TO_STEAMID(user), StringCast<ANSICHAR>(*apiName).Get(), value, length);
}

bool UAPIAppServer::SteamInitWorkshopForGameServer(int32 workshopDepotID, FString absoluteFolderPath)
{
	STEAM_SERVER(false);
	return SteamGameServerUGC()->BInitWorkshopForGameServer(static_cast<DepotId_t>(workshopDepotID), StringCast<ANSICHAR>(*absoluteFolderPath).Get());
}

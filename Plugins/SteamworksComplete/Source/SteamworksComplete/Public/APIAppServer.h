/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <SteamGameInstance.h>
#include "APIAppServer.generated.h"

UCLASS()
class STEAMWORKSCOMPLETE_API UAPIAppServer : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerAssociateWithClan(int64 clanId, FAssociateWithClanResult callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UEBeginAuthSessionResult SteamServerBeginAuthSession(TArray<uint8> authTicket, int64 steamId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamServerLoggedOn();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamServerSecure();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerCancelAuthTicket(int32 authTicketHandle);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerClearAllKeyValues();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerComputeNewPlayerCompatibility(int64 userId, FComputeNewPlayerCompatibilityResult callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetAdvertiseServerActive(bool active);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerEndAuthSession(int64 steamId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FAuthTicketData SteamServerGetAuthSessionTicket(int64 forSteamId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamServerGetPublicIP();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamServerGetSteamID();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamServerInitialize(FString ip, int32 gamePort, int32 queryPort, UEServerMode mode, FString version);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerRunCallbacks();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetServerSettings(FString name, FString description, FString product, FString map, FString modDirectory, bool passwordProtected, bool isDedicated, int maxPlayers, int maxBots, FString gameData, FString gameTags, TMap<FString, FString> keyValues);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerLogOff();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerLogOn(FString token);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerLogOnAnonymous();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamServerRequestUserGroupStatus(int64 userId, int64 groupId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetBotPlayerCount(int botPlayers);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetDedicatedServer(bool dedicated);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetGameData(FString data);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetDescription(FString description);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetGameTags(FString tags);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetKeyValue(FString key, FString value);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetMapName(FString name);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetMaxPlayerCount(int32 playersMax);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetModDirectoryName(FString directoryName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetPasswordProtected(bool isPasswordProtected);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetProduct(FString product);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetRegion(FString region);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetName(FString name);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetSpectatorPort(int32 port);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerSetSpectatorName(FString name);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UEUserHasLicenseForAppResult SteamServerUserHasLicenseForApp(int64 user, int32 app);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamServerWasRestartRequested();

	UFUNCTION(BlueprintCallable, Category = "Steam User Stats Server")
	static bool SteamServerClearUserAchievement(int64 user, FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steam User Stats Server")
	static FServerAchievementStatus SteamServerGetUserAchievement(int64 user, FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steam User Stats Server")
	static FServerStatIntValue SteamServerGetUserIntStat(int64 user, FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steam User Stats Server")
	static FServerStatFloatValue SteamServerGetUserFloatStat(int64 user, FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steam User Stats Server")
	static void SteamServerRequestUserStats(int64 user, FGSStatsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steam User Stats Server")
	static bool SteamServerSetUserAchievement(int64 user, FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steam User Stats Server")
	static bool SteamServerSetUserIntStat(int64 user, FString apiName, int32 value);

	UFUNCTION(BlueprintCallable, Category = "Steam User Stats Server")
	static bool SteamServerSetUserFloatStat(int64 user, FString apiName, float value);

	UFUNCTION(BlueprintCallable, Category = "Steam User Stats Server")
	static void SteamServerStoreUserStats(int64 user, FGSStatsCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steam User Stats Server")
	static bool SteamServerUpdateUserAvgRateStat(int64 user, FString apiName, float value, double length);

	UFUNCTION(BlueprintCallable, Category = "Steam User Stats Server")
	static bool SteamInitWorkshopForGameServer(int32 workshopDepotID, FString absoluteFolderPath);
};

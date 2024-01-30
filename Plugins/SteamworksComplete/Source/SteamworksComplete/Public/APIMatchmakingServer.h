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
THIRD_PARTY_INCLUDES_START
#include <../sdk/isteammatchmaking.h>
THIRD_PARTY_INCLUDES_END
#include "APIMatchmakingServer.generated.h"

UCLASS()
class STEAMWORKSCOMPLETE_API UAPIMatchmakingServer : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerPingServer(FString address, int32 port, FSteamMatchmakingPingResponse callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerPlayerDetails(FString address, int32 port, FSteamMatchmakingPlayersResponse callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerRequestFavoritesServerList(int32 appId, TArray<FKeyValuePair> filter, FSteamServerListResponce callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerRequestFriendsServerList(int32 appId, TArray<FKeyValuePair> filter, FSteamServerListResponce callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerRequestHistoryServerList(int32 appId, TArray<FKeyValuePair> filter, FSteamServerListResponce callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerRequestInternetServerList(int32 appId, TArray<FKeyValuePair> filter, FSteamServerListResponce callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerRequestLANServerList(int32 appId, FSteamServerListResponce callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerRequestSpectatorServerList(int32 appId, TArray<FKeyValuePair> filter, FSteamServerListResponce callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamServerRules(FString address, int32 port, FSteamMatchmakingRulesResponse callback);
};

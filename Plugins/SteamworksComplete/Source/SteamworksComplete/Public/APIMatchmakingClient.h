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
#include "APIMatchmakingClient.generated.h"

UCLASS()
class STEAMWORKSCOMPLETE_API UAPIMatchmakingClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamAddFavoriteGame(int32 appId, FString ip, int32 connectionPort, int32 queryPort, FDateTime lastPlayed);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamAddHistoryGame(int32 appId, FString ip, int32 connectionPort, int32 queryPort, FDateTime lastPlayed);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddRequestLobbyListDistanceFilter(UELobbyDistanceFilter filter);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddRequestLobbyListFilterSlotsAvailable(int32 slotsAvailable);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddRequestLobbyListNearValueFilter(FString key, int32 valueToBeCloseTo);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddRequestLobbyListNumericalFilter(FString key, int32 valueToMatch, UELobbyComparison comparisonType);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddRequestLobbyListResultCountFilter(int32 maxResults);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddRequestLobbyListStringFilter(FString key, FString valueToMatch, UELobbyComparison comparisonType);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamCreateLobby(UELobbyType type, int32 maxMembers, FLobbyCreate callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamDeleteLobbyData(int64 lobbyId, FString key);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FFavoriteGame SteamGetFavoriteGame(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetFavoriteGameCount();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetLobbyByIndex(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FChatEntry SteamGetLobbyChatEntry(int64 lobbyId, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetLobbyData(int64 lobbyId, FString key);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FLobbyData SteamGetLobbyDataByIndex(int64 lobbyId, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetLobbyDataCount(int64 lobbyId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FLobbyGameServer SteamGetLobbyGameServer(int64 lobbyId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetLobbyMemberByIndex(int64 lobbyId, int index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetLobbyMemberData(int64 lobbyId, int64 userId, FString key);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetLobbyMemberLimit(int64 lobbyId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetLobbyOwner(int64 lobbyId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsLobbyOwner(int64 lobbyId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetNumLobbyMembers(int64 lobbyId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<int64> SteamGetLobbyMembers(int64 lobbyId, bool includeLocalUser);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamInviteUserToLobby(int64 lobbyId, int64 userId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamJoinLobby(int64 lobbyId, FLobbyEnter callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamLeaveLobby(int64 lobbyId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamQuickMatch(FLobbyMatch callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamRemoveFavoriteGame(int32 appId, FString ip, int32 connectionPort, int32 queryPort);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamRemoveHistoryGame(int32 appId, FString ip, int32 connectionPort, int32 queryPort);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamRequestLobbyData(int64 lobbyId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamRequestLobbyList(FLobbyMatchList callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSendLobbyChatData(int64 lobbyId, TArray<uint8> data);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSendLobbyChatMessage(int64 lobbyId, FString message);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetLobbyData(int64 lobbyId, FString key, FString value);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSetLobbyGameHost(int64 lobbyId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSetLobbyGameServerId(int64 lobbyId, int64 serverId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSetLobbyGameServerAddress(int64 lobbyId, FString ip, int32 port);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSetLobbyGameServer(int64 lobbyId, int64 serverId, FString ip, int32 port);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetLobbyJoinable(int64 lobbyId, bool joinable);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSetLobbyMemberData(int64 lobbyId, FString key, FString value);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetLobbyMemberLimit(int64 lobbyId, int32 maxMembers);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetLobbyOwner(int64 lobbyId, int64 userId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetLobbyType(int64 lobbyId, UELobbyType type);
};

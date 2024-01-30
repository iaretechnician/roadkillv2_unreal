/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
THIRD_PARTY_INCLUDES_START
#include <HeathenTools.h>
THIRD_PARTY_INCLUDES_END
#include "SteamGameInstance.generated.h"

/**********************************************
* Defines, common use macros
***********************************************/

#define INT64_TO_STEAMID(int64Value) CSteamID(static_cast<uint64>(int64Value))
#define STEAMID_TO_INT64(steamID) static_cast<int64>(steamID.ConvertToUint64())
#define STEAM_CLIENT(returnValue) \
        if (!SteamUser()) \
        { \
            UE_LOG(LogTemp, Warning, TEXT("Steam API is not initialized or SteamGameInstance is not available")); \
            return returnValue; \
        } 
#define STEAM_CLIENT_INSTANCE(returnValue) \
        if (!SteamUser()) \
        { \
            UE_LOG(LogTemp, Warning, TEXT("Steam API is not initialized or SteamGameInstance is not available")); \
            return returnValue; \
        } \
		USteamGameInstance* SteamGameInstance = USteamGameInstance::GetSteamGameInstance(); \
        if (!SteamGameInstance) \
        { \
            UE_LOG(LogTemp, Warning, TEXT("SteamGameInstance is not available")); \
            return returnValue; \
        }
#define STEAM_SERVER(returnValue) \
        if (!SteamGameServer()) \
        { \
            UE_LOG(LogTemp, Warning, TEXT("Steam API is not initialized or SteamGameInstance is not available")); \
            return returnValue; \
        } 
#define STEAM_SERVER_INSTANCE(returnValue) \
        if (!SteamGameServer()) \
        { \
            UE_LOG(LogTemp, Warning, TEXT("Steam API is not initialized or SteamGameInstance is not available")); \
            return returnValue; \
        } \
		USteamGameInstance* SteamGameInstance = USteamGameInstance::GetSteamGameInstance(); \
        if (!SteamGameInstance) \
        { \
            UE_LOG(LogTemp, Warning, TEXT("SteamGameInstance is not available")); \
            return returnValue; \
        }
#define STEAMWORKS_ICON_SIZE 256
#define STEAMWORKS_ICON_BYTE_SIZE STEAMWORKS_ICON_SIZE * STEAMWORKS_ICON_SIZE * 4
#define STEAMWORKS_AVATAR_SIZE 184
#define STEAMWORKS_AVATAR_BYTE_SIZE STEAMWORKS_AVATAR_SIZE * STEAMWORKS_AVATAR_SIZE * 4

DECLARE_DELEGATE_OneParam(SteamGameServerConnectionChangeDelegate, bool);

class FItemResultRequest
{
public:
	/// <summary>
	/// Only used when deserializing inventory results
	/// </summary>
	CSteamID userId;
	/// <summary>
	/// Callback to be executed when the results are finalized
	/// </summary>
	FItemResultsCallback callback;
	/// <summary>
	/// The list of property keys to read for on each item
	/// </summary>
	TArray<FString> properties;
};

UCLASS()
class STEAMWORKSCOMPLETE_API USteamGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Shutdown() override;
	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static USteamGameInstance* GetSteamGameInstance();

	UFUNCTION(BlueprintCallable, Category = "Steam Game Instance")
	FSteamworksInitializationResponse InitializeSteamAPI();

	UFUNCTION(BlueprintCallable, Category = "Steam Game Instance")
	void InitializeRelayNetworkAccess();

	UFUNCTION(BlueprintCallable, Category = "Steam Game Instance")
	void InitializeNetworkingDebugOutput();

	UPROPERTY(EditAnywhere, Category = "General")
	float callbackFrequency = 0.03333f;

	UPROPERTY(EditAnywhere, Category = "General")
	int32 appId = 480;

	UPROPERTY(EditAnywhere, Category = "Server Settings")
	FString ip = FString("0.0.0.0");

	UPROPERTY(EditAnywhere, Category = "Server Settings")
	int32 gamePort = 27017;

	UPROPERTY(EditAnywhere, Category = "Server Settings")
	int32 queryPort = 27018;

	UPROPERTY(EditAnywhere, Category = "Server Settings")
	UEServerMode mode = UEServerMode::EPC_AuthenticationAndSecure;

	UPROPERTY(EditAnywhere, Category = "Server Settings")
	FString version = FString("1.0.0.0");

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FUserStatsReceived SteamUserStatsReceived;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FUserStatsUnloaded SteamUserStatsUnloaded;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FUserStatsUnloaded SteamUserStatsStored;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FUserAchievementStored SteamUserAchievementStored;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FDlcInstalledEvent SteamDlcInstalled;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FSteamServerConnectFailureCallback SteamServerConnectFailure;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FSteamServersDisconnectedCallback SteamServersDisconnected;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FSteamServersConnectedCallback SteamServersConnected;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FGameConnectedFriendChatMsgCallback SteamFriendChatMsg;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FFriendRichPresenceUpdateCallback SteamFriendRichPresenceUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FPersonaStateChangeCallback SteamPersonaStateChange;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FGameRichPresenceJoinRequestedCallback SteamRichPresenceJoinRequested;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FGameLobbyJoinRequestedCallback SteamLobbyJoinRequested;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FGameOverlayActivatedCallback SteamGameOverlayActivated;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FSteamInventoryResultReadyCallback SteamInventoryResultReady;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FSteamInventoryDefinitionUpdateCallback SteamInventoryDefinitionUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FMicroTxnAuthorizationResponseCallback SteamMicroTxnAuthorizationResponse;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FFavoritesListAccountsUpdatedCallback SteamFavoritesListAccountsUpdated;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FFavoritesListChangedCallback SteamFavoritesListChanged;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FLobbyChatMsgCallback SteamLobbyChatMsg;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FLobbyChatUpdateCallback SteamLobbyChatUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FLobbyDataUpdateCallback SteamLobbyDataUpdate;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FLobbyEnterCallback SteamLobbyEnter;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FLobbyGameCreatedCallback SteamLobbyGameCreated;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete");
	FLobbyInviteCallback SteamLobbyInvite;

	UPROPERTY(BlueprintAssignable, Category = "Steamworks Complete")
	FGetTicketForWebApiResponseCallback SteamTicketForWebApiResponse;

	void LoadIcon(int handle, FString apiName, FIconLoadCallback callback);
	void GetAvatarBySteamId(CSteamID SteamId, FAvatarLoadCallback Callback);
	void LoadSteamAvatar(CSteamID SteamId);
	void CopySteamAvatar(int Handle, UTexture2D* AvatarTexture);

	TMap<FString, UTexture2D*> AchievementIcons;
	TMap<FString, FIconLoadCallback> AchievementIconRequests;
	TMap<uint64, UTexture2D*> Avatars;
	TMap<uint64, FAvatarLoadCallback> AvatarRequests;
	TMap<SteamInventoryResult_t, FItemResultRequest> InventoryRequests;

	SteamGameServerConnectionChangeDelegate SteamGameServerConnectionChanged;

private:
	static USteamGameInstance* pInstance;

	FTimerHandle SteamCallbackTimerHandle;

	STEAM_CALLBACK(USteamGameInstance, EventUserStatsReceived, UserStatsReceived_t);
	STEAM_CALLBACK(USteamGameInstance, EventUserStatsUnloaded, UserStatsUnloaded_t);
	STEAM_CALLBACK(USteamGameInstance, EventUserStatsStored, UserStatsStored_t);
	STEAM_CALLBACK(USteamGameInstance, EventUserAchievementStored, UserAchievementStored_t);
	STEAM_CALLBACK(USteamGameInstance, EventUserAchievementIconFetched, UserAchievementIconFetched_t);
	STEAM_CALLBACK(USteamGameInstance, EventDlcInstalled, DlcInstalled_t);
	STEAM_CALLBACK(USteamGameInstance, EventSteamServerConnectFailure, SteamServerConnectFailure_t);
	STEAM_CALLBACK(USteamGameInstance, EventSteamServersConnected, SteamServersConnected_t);
	STEAM_CALLBACK(USteamGameInstance, EventSteamServersDisconnected, SteamServersDisconnected_t);
	STEAM_CALLBACK(USteamGameInstance, EventGameConnectedFriendChatMsg, GameConnectedFriendChatMsg_t);
	STEAM_CALLBACK(USteamGameInstance, EventFriendRichPresenceUpdate, FriendRichPresenceUpdate_t);
	STEAM_CALLBACK(USteamGameInstance, EventPersonaStateChange, PersonaStateChange_t);
	STEAM_CALLBACK(USteamGameInstance, EventAvatarImageLoaded, AvatarImageLoaded_t);
	STEAM_CALLBACK(USteamGameInstance, EventGameRichPresenceJoinRequested, GameRichPresenceJoinRequested_t);
	STEAM_CALLBACK(USteamGameInstance, EventGameLobbyJoinRequested, GameLobbyJoinRequested_t);
	STEAM_CALLBACK(USteamGameInstance, EventGameOverlayActivated, GameOverlayActivated_t);
	STEAM_CALLBACK(USteamGameInstance, EventSteamInventoryResultReady_t, SteamInventoryResultReady_t);
	STEAM_CALLBACK(USteamGameInstance, EventSteamInventoryDefinitionUpdate_t, SteamInventoryDefinitionUpdate_t);
	STEAM_CALLBACK(USteamGameInstance, EventMicroTxnAuthorizationResponse_t, MicroTxnAuthorizationResponse_t);
	STEAM_CALLBACK(USteamGameInstance, EventFavoritesListAccountsUpdated, FavoritesListAccountsUpdated_t);
	STEAM_CALLBACK(USteamGameInstance, EventFavoritesListChanged, FavoritesListChanged_t);
	STEAM_CALLBACK(USteamGameInstance, EventLobbyChatMsg, LobbyChatMsg_t);
	STEAM_CALLBACK(USteamGameInstance, EventLobbyChatUpdate, LobbyChatUpdate_t);
	STEAM_CALLBACK(USteamGameInstance, EventLobbyDataUpdate, LobbyDataUpdate_t);
	STEAM_CALLBACK(USteamGameInstance, EventLobbyEnter, LobbyEnter_t);
	STEAM_CALLBACK(USteamGameInstance, EventLobbyGameCreated, LobbyGameCreated_t);
	STEAM_CALLBACK(USteamGameInstance, EventLobbyInvite, LobbyInvite_t);
	STEAM_CALLBACK(USteamGameInstance, EventGetTicketForWebApiResponse, GetTicketForWebApiResponse_t);

	//STEAM_GAMESERVER_CALLBACK
	STEAM_GAMESERVER_CALLBACK(USteamGameInstance, EventSteamGameServerConnectFailure, SteamServerConnectFailure_t);
	STEAM_GAMESERVER_CALLBACK(USteamGameInstance, EventSteamGameServersConnected, SteamServersConnected_t);
	STEAM_GAMESERVER_CALLBACK(USteamGameInstance, EventSteamGameServersDisconnected, SteamServersDisconnected_t);
};

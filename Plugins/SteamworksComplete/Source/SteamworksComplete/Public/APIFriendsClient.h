/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include <SteamGameInstance.h>
#include "APIFriendsClient.generated.h"

UCLASS()
class STEAMWORKSCOMPLETE_API UAPIFriendsClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamActivateGameOverlayByName(FString dialog);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamActivateGameOverlay(UEOverlayDialog dialog);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamActivateGameOverlayInviteDialog(int64 lobbyId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamActivateGameOverlayToStore(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamActivateGameOverlayAddToCartAndShow(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamActivateGameOverlayToUser(int64 userId, UEOverlayUserDialog dialog);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamActivateGameOverlayToWebPage(FString url);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamActivateGameOverlayToWebPageModal(FString url);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsOverlayEnabled();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamOverlayNotificationPosition(UEOverlayCorner position);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamOverlayNotificationInset(int32 horizontal, int32 vertical);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSetListenForFriendsMessages(bool enable);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSetPersonaName(FString name, FSetPersonaNameResponseCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetPersonaName();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UEPersonaState SteamGetPersonaState();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UEUserRestrition SteamGetUserRestrictions();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamGetFollowed(FFriendsEnumerateFollowingListCallback Callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetCoplayFriend(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetCoplayFriendCount();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<int64> SteamGetCoplayFriends();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetFriendByIndex(int32 index, UEFriendFlags flag);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetFriendCoplayGame(int64 userId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FDateTime SteamGetFriendCoplayTime(int64 userId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetFriendCount(UEFriendFlags flag);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<int64> SteamGetFriends(UEFriendFlags flag);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetFriendCountFromSource(int64 sourceId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetFriendFromSourceByIndex(int64 sourceId, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<int64> SteamGetFriendFromSource(int64 sourceId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FMyGameInfo SteamGetFriendGamePlayed(int64 userId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetFriendGroupIdByIndex(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetFriendGroupsCount();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<int32> SteamGetFriendGroups();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<int64> SteamGetFriendGroupMembers(int32 friendGroupId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetFriendGroupName(int32 friendGroupId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamHasFriend(int64 steamId, UEFriendFlags flag);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamInviteUserToGame(int64 userId, FString connectionString);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamReplyToFriendMessage(int64 userId, FString message);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamRequestFriendRichPresence(int64 userId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamRequestUserInformation(int64 userId, bool nameOnly);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSetInGameVoiceSpeaking(bool speaking);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSetPlayedWithUser(int64 userId);

	UFUNCTION(BlueprintCallable, Category = "Steam User Client")
	static FString SteamGetFriendPersonaName(int64 id);

	UFUNCTION(BlueprintCallable, Category = "Steam User Client")
	static FString SteamGetPlayerNickname(int64 id);

	UFUNCTION(BlueprintCallable, Category = "Steam User Client")
	static bool SteamIsValidSteamUserId(int64 id);

	UFUNCTION(BlueprintCallable, Category = "Steam User Client")
	static void SteamGetMySteamAvatar(FAvatarLoadCallback Callback);

	UFUNCTION(BlueprintCallable, Category = "Steam User Client")
	static void SteamGetUserSteamAvatar(int64 id, FAvatarLoadCallback Callback);
};

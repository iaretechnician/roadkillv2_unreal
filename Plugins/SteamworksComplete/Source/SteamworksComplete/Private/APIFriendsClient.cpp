/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "APIFriendsClient.h"

void UAPIFriendsClient::SteamSetListenForFriendsMessages(bool enable)
{
	STEAM_CLIENT(;);
	SteamFriends()->SetListenForFriendsMessages(enable);
}

void UAPIFriendsClient::SteamSetPersonaName(FString name, FSetPersonaNameResponseCallback callback)
{
	STEAM_CLIENT(;);
	SetPersonaNameResponseLinker* linker = new SetPersonaNameResponseLinker(name, callback);
}

FString UAPIFriendsClient::SteamGetPersonaName()
{
	STEAM_CLIENT(FString());
	return FString(SteamFriends()->GetPersonaName());
}

UEPersonaState UAPIFriendsClient::SteamGetPersonaState()
{
	STEAM_CLIENT(UEPersonaState::EPC_Offline);
	return static_cast<UEPersonaState>(SteamFriends()->GetPersonaState());
}

UEUserRestrition UAPIFriendsClient::SteamGetUserRestrictions()
{
	STEAM_CLIENT(UEUserRestrition::EPC_Unknown);
	return static_cast<UEUserRestrition>(SteamFriends()->GetUserRestrictions());
}

void UAPIFriendsClient::SteamGetFollowed(FFriendsEnumerateFollowingListCallback Callback)
{
	if (!SteamFriends())
	{
		if (Callback.IsBound())
		{
			TArray<int64> EmptyArray;
			Callback.Execute(UEResult::EPC_Fail, EmptyArray);
		}
		UE_LOG(LogTemp, Warning, TEXT("Steam API is not initialized"));
		return;
	}
	APIFriendsClientFriendsEnumerateFollowingListLinker* Linker = new APIFriendsClientFriendsEnumerateFollowingListLinker(Callback);
}

int64 UAPIFriendsClient::SteamGetCoplayFriend(int32 index)
{
	STEAM_CLIENT(0);
	CSteamID fId = SteamFriends()->GetCoplayFriend(index);
	return static_cast<int64>(fId.ConvertToUint64());
}

int32 UAPIFriendsClient::SteamGetCoplayFriendCount()
{
	STEAM_CLIENT(0);

	return SteamFriends()->GetCoplayFriendCount();
}

TArray<int64> UAPIFriendsClient::SteamGetCoplayFriends()
{
	STEAM_CLIENT(TArray<int64>());
	
	TArray<int64> friendArray;

	int friendCount = SteamFriends()->GetCoplayFriendCount();
	for (int i = 0; i < friendCount; i++)
	{
		CSteamID fId = SteamFriends()->GetCoplayFriend(i);
		friendArray.Add(static_cast<int64>(fId.ConvertToUint64()));
	}
	return friendArray;
}

int64 UAPIFriendsClient::SteamGetFriendByIndex(int32 index, UEFriendFlags flag)
{
	STEAM_CLIENT(0);

	int eFlag = EFriendFlags::k_EFriendFlagImmediate;
	switch (flag)
	{
	case UEFriendFlags::EPC_None:
		eFlag = EFriendFlags::k_EFriendFlagNone;
		break;
	case UEFriendFlags::EPC_Blocked:
		eFlag = EFriendFlags::k_EFriendFlagBlocked;
		break;
	case UEFriendFlags::EPC_ChatMember:
		eFlag = EFriendFlags::k_EFriendFlagChatMember;
		break;
	case UEFriendFlags::EPC_ClanMember:
		eFlag = EFriendFlags::k_EFriendFlagClanMember;
		break;
	case UEFriendFlags::EPC_FriendshipRequested:
		eFlag = EFriendFlags::k_EFriendFlagFriendshipRequested;
		break;
	case UEFriendFlags::EPC_Ignored:
		eFlag = EFriendFlags::k_EFriendFlagIgnored;
		break;
	case UEFriendFlags::EPC_IgnoredFriend:
		eFlag = EFriendFlags::k_EFriendFlagIgnoredFriend;
		break;
	case UEFriendFlags::EPC_Immediate:
		eFlag = EFriendFlags::k_EFriendFlagImmediate;
		break;
	}
	CSteamID fId = SteamFriends()->GetFriendByIndex(index, eFlag);
	return int64();
}

int32 UAPIFriendsClient::SteamGetFriendCoplayGame(int64 userId)
{
	STEAM_CLIENT(0);

	CSteamID fId(static_cast<uint64>(userId));
	AppId_t appId = SteamFriends()->GetFriendCoplayGame(fId);

	return static_cast<int32>(appId);
}

FDateTime UAPIFriendsClient::SteamGetFriendCoplayTime(int64 userId)
{
	STEAM_CLIENT(FDateTime::MinValue());

	CSteamID fId(static_cast<uint64>(userId));
	return FDateTime::FromUnixTimestamp(SteamFriends()->GetFriendCoplayTime(fId));
}

int32 UAPIFriendsClient::SteamGetFriendCount(UEFriendFlags flag)
{
	STEAM_CLIENT(0);

	int eFlag = EFriendFlags::k_EFriendFlagImmediate;
	switch (flag)
	{
	case UEFriendFlags::EPC_None:
		eFlag = EFriendFlags::k_EFriendFlagNone;
		break;
	case UEFriendFlags::EPC_Blocked:
		eFlag = EFriendFlags::k_EFriendFlagBlocked;
		break;
	case UEFriendFlags::EPC_ChatMember:
		eFlag = EFriendFlags::k_EFriendFlagChatMember;
		break;
	case UEFriendFlags::EPC_ClanMember:
		eFlag = EFriendFlags::k_EFriendFlagClanMember;
		break;
	case UEFriendFlags::EPC_FriendshipRequested:
		eFlag = EFriendFlags::k_EFriendFlagFriendshipRequested;
		break;
	case UEFriendFlags::EPC_Ignored:
		eFlag = EFriendFlags::k_EFriendFlagIgnored;
		break;
	case UEFriendFlags::EPC_IgnoredFriend:
		eFlag = EFriendFlags::k_EFriendFlagIgnoredFriend;
		break;
	case UEFriendFlags::EPC_Immediate:
		eFlag = EFriendFlags::k_EFriendFlagImmediate;
		break;
	}

	return SteamFriends()->GetFriendCount(eFlag);
}

TArray<int64> UAPIFriendsClient::SteamGetFriends(UEFriendFlags flag)
{
	STEAM_CLIENT(TArray<int64>());

	int eFlag = EFriendFlags::k_EFriendFlagImmediate;
	switch (flag)
	{
	case UEFriendFlags::EPC_None:
		eFlag = EFriendFlags::k_EFriendFlagNone;
		break;
	case UEFriendFlags::EPC_Blocked:
		eFlag = EFriendFlags::k_EFriendFlagBlocked;
		break;
	case UEFriendFlags::EPC_ChatMember:
		eFlag = EFriendFlags::k_EFriendFlagChatMember;
		break;
	case UEFriendFlags::EPC_ClanMember:
		eFlag = EFriendFlags::k_EFriendFlagClanMember;
		break;
	case UEFriendFlags::EPC_FriendshipRequested:
		eFlag = EFriendFlags::k_EFriendFlagFriendshipRequested;
		break;
	case UEFriendFlags::EPC_Ignored:
		eFlag = EFriendFlags::k_EFriendFlagIgnored;
		break;
	case UEFriendFlags::EPC_IgnoredFriend:
		eFlag = EFriendFlags::k_EFriendFlagIgnoredFriend;
		break;
	case UEFriendFlags::EPC_Immediate:
		eFlag = EFriendFlags::k_EFriendFlagImmediate;
		break;
	}

	int count = SteamFriends()->GetFriendCount(eFlag);
	TArray<int64> results;

	for (int i = 0; i < count; i++)
	{
		results.Add(STEAMID_TO_INT64(SteamFriends()->GetFriendByIndex(i, eFlag)));
	}

	return results;
}

int32 UAPIFriendsClient::SteamGetFriendCountFromSource(int64 sourceId)
{
	STEAM_CLIENT(0);

	CSteamID sId(static_cast<uint64>(sourceId));
	return SteamFriends()->GetFriendCountFromSource(sId);
}

int64 UAPIFriendsClient::SteamGetFriendFromSourceByIndex(int64 sourceId, int32 index)
{
	STEAM_CLIENT(0);

	CSteamID sId(static_cast<uint64>(sourceId));
	CSteamID fId = SteamFriends()->GetFriendFromSourceByIndex(sId, index);

	return static_cast<int64>(fId.ConvertToUint64());
}

TArray<int64> UAPIFriendsClient::SteamGetFriendFromSource(int64 sourceId)
{
	STEAM_CLIENT(TArray<int64>());

	TArray<int64> friendArray;

	CSteamID sId(static_cast<uint64>(sourceId));
	int count = SteamFriends()->GetFriendCountFromSource(sId);
	for (int i = 0; i < count; i++)
	{
		CSteamID fId = SteamFriends()->GetFriendFromSourceByIndex(sId, i);
		friendArray.Add(static_cast<int64>(fId.ConvertToUint64()));
	}

	return friendArray;
}

FMyGameInfo UAPIFriendsClient::SteamGetFriendGamePlayed(int64 userId)
{
	STEAM_CLIENT(FMyGameInfo());

	CSteamID fId(static_cast<uint64>(userId));
	FriendGameInfo_t friendGameInfo;

	bool inGame = SteamFriends()->GetFriendGamePlayed(fId, &friendGameInfo);

	FMyGameInfo gameInfo;
	gameInfo.InGame = inGame;

	if (inGame)
	{
		uint8 Octets[4];
		Octets[0] = (friendGameInfo.m_unGameIP >> 24) & 0xFF;
		Octets[1] = (friendGameInfo.m_unGameIP >> 16) & 0xFF;
		Octets[2] = (friendGameInfo.m_unGameIP >> 8) & 0xFF;
		Octets[3] = friendGameInfo.m_unGameIP & 0xFF;

		gameInfo.IpAddress = FString::Printf(TEXT("%d.%d.%d.%d"), Octets[0], Octets[1], Octets[2], Octets[3]);
		gameInfo.GamePort = static_cast<int32>(friendGameInfo.m_usGamePort);
		gameInfo.QueryPort = static_cast<int32>(friendGameInfo.m_usQueryPort);
		gameInfo.GameId = static_cast<int64>(friendGameInfo.m_gameID.ToUint64());
		gameInfo.LobbyId = static_cast<int64>(friendGameInfo.m_steamIDLobby.ConvertToUint64());
	}

	return gameInfo;
}

int32 UAPIFriendsClient::SteamGetFriendGroupIdByIndex(int32 index)
{
	STEAM_CLIENT(0);
	
	return static_cast<int32>(SteamFriends()->GetFriendsGroupIDByIndex(index));
}

TArray<int32> UAPIFriendsClient::SteamGetFriendGroups()
{
	STEAM_CLIENT(TArray<int32>());

	TArray<int32> results;
	int count = SteamFriends()->GetFriendsGroupCount();
	for (int i = 0; i < count; i++)
	{
		FriendsGroupID_t fGroupId = SteamFriends()->GetFriendsGroupIDByIndex(i);
		results.Add(static_cast<int32>(fGroupId));
	}

	return results;
}

int32 UAPIFriendsClient::SteamGetFriendGroupsCount()
{
	STEAM_CLIENT(0);
	return SteamFriends()->GetFriendsGroupCount();
}

TArray<int64> UAPIFriendsClient::SteamGetFriendGroupMembers(int32 friendGroupId)
{
	STEAM_CLIENT(TArray<int64>());

	FriendsGroupID_t fGroupId = static_cast<FriendsGroupID_t>(friendGroupId);
	int count = SteamFriends()->GetFriendsGroupMembersCount(fGroupId);	
	CSteamID* buffer = new CSteamID[count];
	SteamFriends()->GetFriendsGroupMembersList(fGroupId, buffer, count);

	TArray<int64> uArray;

	for (int i = 0; i < count; i++)
	{
		uArray.Add(static_cast<int64>(buffer[i].ConvertToUint64()));
	}

	delete[] buffer;

	return uArray;
}

FString UAPIFriendsClient::SteamGetFriendGroupName(int32 friendGroupId)
{
	STEAM_CLIENT(FString());

	FriendsGroupID_t fGroupId = static_cast<FriendsGroupID_t>(friendGroupId);
	const char* GroupNameBuffer = SteamFriends()->GetFriendsGroupName(fGroupId);

	if (GroupNameBuffer)
	{
		FString GroupName(GroupNameBuffer);
		return GroupName;
	}
	else
		return FString();
}

bool UAPIFriendsClient::SteamHasFriend(int64 steamId, UEFriendFlags flag)
{
	STEAM_CLIENT(false);
	return SteamFriends()->HasFriend(INT64_TO_STEAMID(steamId), HeathenTools::Convert(flag));
}

bool UAPIFriendsClient::SteamInviteUserToGame(int64 userId, FString connectionString)
{
	STEAM_CLIENT(false);
	CSteamID steamId = INT64_TO_STEAMID(userId);
	bool result = SteamFriends()->InviteUserToGame(steamId, StringCast<ANSICHAR>(*connectionString).Get());
	return result;
}

bool UAPIFriendsClient::SteamReplyToFriendMessage(int64 userId, FString message)
{
	STEAM_CLIENT(false);
	CSteamID steamId = INT64_TO_STEAMID(userId);
	return SteamFriends()->ReplyToFriendMessage(steamId, StringCast<ANSICHAR>(*message).Get());
}

void UAPIFriendsClient::SteamRequestFriendRichPresence(int64 userId)
{
	STEAM_CLIENT(;);
	CSteamID steamId = INT64_TO_STEAMID(userId);

	SteamFriends()->RequestFriendRichPresence(steamId);
}

bool UAPIFriendsClient::SteamRequestUserInformation(int64 userId, bool nameOnly)
{
	STEAM_CLIENT(false);
	CSteamID steamId = INT64_TO_STEAMID(userId);

	return SteamFriends()->RequestUserInformation(steamId, nameOnly);
}

void UAPIFriendsClient::SteamSetInGameVoiceSpeaking(bool speaking)
{
	STEAM_CLIENT(;);
	SteamFriends()->SetInGameVoiceSpeaking(CSteamID(), speaking);
}

void UAPIFriendsClient::SteamSetPlayedWithUser(int64 userId)
{
	STEAM_CLIENT(;);
	CSteamID steamId = INT64_TO_STEAMID(userId);
	SteamFriends()->SetPlayedWith(steamId);
}

FString UAPIFriendsClient::SteamGetFriendPersonaName(int64 id)
{
	STEAM_CLIENT(FString());
	return FString(SteamFriends()->GetFriendPersonaName(INT64_TO_STEAMID(id)));
}

FString UAPIFriendsClient::SteamGetPlayerNickname(int64 id)
{
	STEAM_CLIENT(FString());
	return FString(SteamFriends()->GetPlayerNickname(INT64_TO_STEAMID(id)));
}

void UAPIFriendsClient::SteamGetMySteamAvatar(FAvatarLoadCallback Callback)
{
	STEAM_CLIENT_INSTANCE(;);
	SteamGameInstance->GetAvatarBySteamId(SteamUser()->GetSteamID(), Callback);
}

bool UAPIFriendsClient::SteamIsValidSteamUserId(int64 id)
{
	return INT64_TO_STEAMID(id).IsValid();
}

void UAPIFriendsClient::SteamGetUserSteamAvatar(int64 id, FAvatarLoadCallback Callback)
{
	STEAM_CLIENT_INSTANCE(;);
	SteamGameInstance->GetAvatarBySteamId(INT64_TO_STEAMID(id), Callback);
}

void UAPIFriendsClient::SteamActivateGameOverlayByName(FString dialog)
{
	STEAM_CLIENT_INSTANCE(;);
	SteamFriends()->ActivateGameOverlay(StringCast<ANSICHAR>(*dialog).Get());
}

void UAPIFriendsClient::SteamActivateGameOverlay(UEOverlayDialog dialog)
{
	STEAM_CLIENT_INSTANCE(;);
	switch (dialog)
	{
	case UEOverlayDialog::EPC_Achievements:
		SteamFriends()->ActivateGameOverlay("achievements");
		break;
	case UEOverlayDialog::EPC_Community:
		SteamFriends()->ActivateGameOverlay("community");
		break;
	case UEOverlayDialog::EPC_Friends:
		SteamFriends()->ActivateGameOverlay("friends");
		break;
	case UEOverlayDialog::EPC_OfficialGameGroup:
		SteamFriends()->ActivateGameOverlay("officialgamegroup");
		break;
	case UEOverlayDialog::EPC_Players:
		SteamFriends()->ActivateGameOverlay("players");
		break;
	case UEOverlayDialog::EPC_Settings:
		SteamFriends()->ActivateGameOverlay("settings");
		break;
	case UEOverlayDialog::EPC_Stats:
		SteamFriends()->ActivateGameOverlay("stats");
		break;
	}
}

void UAPIFriendsClient::SteamActivateGameOverlayInviteDialog(int64 lobbyId)
{
	STEAM_CLIENT_INSTANCE(;);
	SteamFriends()->ActivateGameOverlayInviteDialog(INT64_TO_STEAMID(lobbyId));
}

void UAPIFriendsClient::SteamActivateGameOverlayToStore(int32 appId)
{
	STEAM_CLIENT_INSTANCE(;);
	SteamFriends()->ActivateGameOverlayToStore(static_cast<AppId_t>(appId), EOverlayToStoreFlag::k_EOverlayToStoreFlag_None);
}

void UAPIFriendsClient::SteamActivateGameOverlayAddToCartAndShow(int32 appId)
{
	STEAM_CLIENT_INSTANCE(;);
	SteamFriends()->ActivateGameOverlayToStore(static_cast<AppId_t>(appId), EOverlayToStoreFlag::k_EOverlayToStoreFlag_AddToCartAndShow);
}

void UAPIFriendsClient::SteamActivateGameOverlayToUser(int64 userId, UEOverlayUserDialog dialog)
{
	STEAM_CLIENT_INSTANCE(;);
	SteamFriends()->ActivateGameOverlayToUser("", INT64_TO_STEAMID(userId));
}

void UAPIFriendsClient::SteamActivateGameOverlayToWebPage(FString url)
{
	STEAM_CLIENT_INSTANCE(;);
	SteamFriends()->ActivateGameOverlayToWebPage(StringCast<ANSICHAR>(*url).Get());
}

void UAPIFriendsClient::SteamActivateGameOverlayToWebPageModal(FString url)
{
	STEAM_CLIENT_INSTANCE(;);
	SteamFriends()->ActivateGameOverlayToWebPage(StringCast<ANSICHAR>(*url).Get(), EActivateGameOverlayToWebPageMode::k_EActivateGameOverlayToWebPageMode_Modal);
}

bool UAPIFriendsClient::SteamIsOverlayEnabled()
{
	STEAM_CLIENT_INSTANCE(false);
	return SteamUtils()->IsOverlayEnabled();
}

void UAPIFriendsClient::SteamOverlayNotificationPosition(UEOverlayCorner position)
{
	STEAM_CLIENT_INSTANCE(;);
	return SteamUtils()->SetOverlayNotificationPosition(static_cast<ENotificationPosition>(position));
}

void UAPIFriendsClient::SteamOverlayNotificationInset(int32 horizontal, int32 vertical)
{
	STEAM_CLIENT_INSTANCE(;);
	return SteamUtils()->SetOverlayNotificationInset(horizontal, vertical);
}

/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "APIMatchmakingClient.h"

int32 UAPIMatchmakingClient::SteamAddFavoriteGame(int32 appId, FString ip, int32 connectionPort, int32 queryPort, FDateTime lastPlayed)
{
	STEAM_CLIENT(0);
	AppId_t app = static_cast<AppId_t>(appId);
	uint32 address = HeathenTools::IPAddress(ip);
	uint16 cPort = static_cast<uint16>(connectionPort);
	uint16 qPort = static_cast<uint16>(queryPort);
	return SteamMatchmaking()->AddFavoriteGame(app, address, cPort, qPort, 0x01, static_cast<uint32>(lastPlayed.ToUnixTimestamp()));
}

int32 UAPIMatchmakingClient::SteamAddHistoryGame(int32 appId, FString ip, int32 connectionPort, int32 queryPort, FDateTime lastPlayed)
{
	STEAM_CLIENT(0);
	AppId_t app = static_cast<AppId_t>(appId);
	uint32 address = HeathenTools::IPAddress(ip);
	uint16 cPort = static_cast<uint16>(connectionPort);
	uint16 qPort = static_cast<uint16>(queryPort);
	return SteamMatchmaking()->AddFavoriteGame(app, address, cPort, qPort, 0x02, static_cast<uint32>(lastPlayed.ToUnixTimestamp()));
}

void UAPIMatchmakingClient::SteamAddRequestLobbyListDistanceFilter(UELobbyDistanceFilter filter)
{
	STEAM_CLIENT(;);
	SteamMatchmaking()->AddRequestLobbyListDistanceFilter(static_cast<ELobbyDistanceFilter>(filter));
}

void UAPIMatchmakingClient::SteamAddRequestLobbyListFilterSlotsAvailable(int32 slotsAvailable)
{
	STEAM_CLIENT(;);
	SteamMatchmaking()->AddRequestLobbyListFilterSlotsAvailable(slotsAvailable);
}

void UAPIMatchmakingClient::SteamAddRequestLobbyListNearValueFilter(FString key, int32 valueToBeCloseTo)
{
	STEAM_CLIENT(;);
	SteamMatchmaking()->AddRequestLobbyListNearValueFilter(StringCast<ANSICHAR>(*key).Get(), valueToBeCloseTo);
}

void UAPIMatchmakingClient::SteamAddRequestLobbyListNumericalFilter(FString key, int32 valueToMatch, UELobbyComparison comparisonType)
{
	STEAM_CLIENT(;);
	ELobbyComparison comp;
	switch (comparisonType)
	{
	case UELobbyComparison::EPC_EqualToOrLessThan:
		comp = ELobbyComparison::k_ELobbyComparisonEqualToOrLessThan;
		break;
	case UELobbyComparison::EPC_LessThan:
		comp = ELobbyComparison::k_ELobbyComparisonLessThan;
		break;
	case UELobbyComparison::EPC_Equal:
		comp = ELobbyComparison::k_ELobbyComparisonEqual;
		break;
	case UELobbyComparison::EPC_GreaterThan:
		comp = ELobbyComparison::k_ELobbyComparisonGreaterThan;
		break;
	case UELobbyComparison::EPC_EqualToOrGreaterThan:
		comp = ELobbyComparison::k_ELobbyComparisonEqualToOrGreaterThan;
		break;
	case UELobbyComparison::EPC_NotEqual:
		comp = ELobbyComparison::k_ELobbyComparisonNotEqual;
		break;
	default:
		comp = ELobbyComparison::k_ELobbyComparisonEqual;
		break;
	}
	SteamMatchmaking()->AddRequestLobbyListNumericalFilter(StringCast<ANSICHAR>(*key).Get(), valueToMatch, comp);
}

void UAPIMatchmakingClient::SteamAddRequestLobbyListResultCountFilter(int32 maxResults)
{
	STEAM_CLIENT(;);
	SteamMatchmaking()->AddRequestLobbyListResultCountFilter(maxResults);
}

void UAPIMatchmakingClient::SteamAddRequestLobbyListStringFilter(FString key, FString valueToMatch, UELobbyComparison comparisonType)
{
	STEAM_CLIENT(;);
	ELobbyComparison comp;
	switch (comparisonType)
	{
	case UELobbyComparison::EPC_EqualToOrLessThan:
		comp = ELobbyComparison::k_ELobbyComparisonEqualToOrLessThan;
		break;
	case UELobbyComparison::EPC_LessThan:
		comp = ELobbyComparison::k_ELobbyComparisonLessThan;
		break;
	case UELobbyComparison::EPC_Equal:
		comp = ELobbyComparison::k_ELobbyComparisonEqual;
		break;
	case UELobbyComparison::EPC_GreaterThan:
		comp = ELobbyComparison::k_ELobbyComparisonGreaterThan;
		break;
	case UELobbyComparison::EPC_EqualToOrGreaterThan:
		comp = ELobbyComparison::k_ELobbyComparisonEqualToOrGreaterThan;
		break;
	case UELobbyComparison::EPC_NotEqual:
		comp = ELobbyComparison::k_ELobbyComparisonNotEqual;
		break;
	default:
		comp = ELobbyComparison::k_ELobbyComparisonEqual;
		break;
	}
	
	SteamMatchmaking()->AddRequestLobbyListStringFilter(StringCast<ANSICHAR>(*key).Get(), StringCast<ANSICHAR>(*valueToMatch).Get(), comp);
}

void UAPIMatchmakingClient::SteamCreateLobby(UELobbyType type, int32 maxMembers, FLobbyCreate callback)
{
	STEAM_CLIENT(;);
	FLobbyCreateLinker* linker = new FLobbyCreateLinker(type, maxMembers, callback);
}

bool UAPIMatchmakingClient::SteamDeleteLobbyData(int64 lobbyId, FString key)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->DeleteLobbyData(INT64_TO_STEAMID(lobbyId), StringCast<ANSICHAR>(*key).Get());
}

FFavoriteGame UAPIMatchmakingClient::SteamGetFavoriteGame(int32 index)
{
	STEAM_CLIENT(FFavoriteGame());
	AppId_t app;
	uint32 ip;
	uint16 qport;
	uint16 cport;
	uint32 flag;
	uint32 time;

	SteamMatchmaking()->GetFavoriteGame(index, &app, &ip, &cport, &qport, &flag, &time);

	FFavoriteGame result;
	result.AppId = static_cast<int32>(app);
	result.ConnectionPort = static_cast<int32>(cport);
	result.QueryPort = static_cast<int32>(qport);
	result.IP = HeathenTools::IPAddress(ip);
	result.LastPlayedOnServer = FDateTime::FromUnixTimestamp(time);
	result.History = flag == 0x01;

	return result;
}

int32 UAPIMatchmakingClient::SteamGetFavoriteGameCount()
{
	STEAM_CLIENT(0);
	return SteamMatchmaking()->GetFavoriteGameCount();
}

int64 UAPIMatchmakingClient::SteamGetLobbyByIndex(int32 index)
{
	STEAM_CLIENT(0);
	CSteamID lobby = SteamMatchmaking()->GetLobbyByIndex(index);
	return STEAMID_TO_INT64(lobby);
}

FChatEntry UAPIMatchmakingClient::SteamGetLobbyChatEntry(int64 lobbyId, int32 index)
{
	STEAM_CLIENT(FChatEntry());
	FChatEntry result;
	CSteamID lobby = INT64_TO_STEAMID(lobbyId);
	CSteamID user = CSteamID();
	TArray<uint8> data;
	data.SetNumUninitialized(4096);
	EChatEntryType type;
	int32 dataLength = SteamMatchmaking()->GetLobbyChatEntry(lobby, index, &user, data.GetData(), 4096, &type);

	data.SetNum(dataLength);
	result.Data = data;
	result.Type = static_cast<UEChatEntryType>(type);
	result.String = FString::FromBlob(data.GetData(), data.Num());
	result.UserId = STEAMID_TO_INT64(user);

	return result;
}

FString UAPIMatchmakingClient::SteamGetLobbyData(int64 lobbyId, FString key)
{
	STEAM_CLIENT(FString());
	return FString(SteamMatchmaking()->GetLobbyData(INT64_TO_STEAMID(lobbyId), StringCast<ANSICHAR>(*key).Get()));
}

FLobbyData UAPIMatchmakingClient::SteamGetLobbyDataByIndex(int64 lobbyId, int32 index)
{
	STEAM_CLIENT(FLobbyData());
	FLobbyData result;
	CSteamID lobby = INT64_TO_STEAMID(lobbyId);
	char keyBuffer[255];
	char valueBuffer[8192];
	bool response = SteamMatchmaking()->GetLobbyDataByIndex(lobby, index, keyBuffer, 255, valueBuffer, 8192);

	if (response)
	{
		result.Key = FString(keyBuffer);
		result.Value = FString(valueBuffer);
	}
	return result;
}

int32 UAPIMatchmakingClient::SteamGetLobbyDataCount(int64 lobbyId)
{
	STEAM_CLIENT(0);
	return SteamMatchmaking()->GetLobbyDataCount(INT64_TO_STEAMID(lobbyId));
}

FLobbyGameServer UAPIMatchmakingClient::SteamGetLobbyGameServer(int64 lobbyId)
{
	STEAM_CLIENT(FLobbyGameServer());
	FLobbyGameServer result;
	CSteamID lobby = INT64_TO_STEAMID(lobbyId);
	uint32 ip = 0;
	uint16 port = 0;
	CSteamID serverId = CSteamID();
	bool response = SteamMatchmaking()->GetLobbyGameServer(lobby, &ip, &port, &serverId);

	result.GameServerId = STEAMID_TO_INT64(serverId);
	result.IP = HeathenTools::IPAddress(ip);
	result.IsSet = response;
	result.Port = static_cast<int32>(port);
	
	return result;
}

int64 UAPIMatchmakingClient::SteamGetLobbyMemberByIndex(int64 lobbyId, int index)
{
	STEAM_CLIENT(0);
	CSteamID userId = SteamMatchmaking()->GetLobbyMemberByIndex(INT64_TO_STEAMID(lobbyId), index);
	return STEAMID_TO_INT64(userId);
}

FString UAPIMatchmakingClient::SteamGetLobbyMemberData(int64 lobbyId, int64 userId, FString key)
{
	STEAM_CLIENT(FString());
	return FString(SteamMatchmaking()->GetLobbyMemberData(INT64_TO_STEAMID(lobbyId), INT64_TO_STEAMID(userId), StringCast<ANSICHAR>(*key).Get()));
}

int32 UAPIMatchmakingClient::SteamGetLobbyMemberLimit(int64 lobbyId)
{
	STEAM_CLIENT(0);
	return SteamMatchmaking()->GetLobbyMemberLimit(INT64_TO_STEAMID(lobbyId));
}

int64 UAPIMatchmakingClient::SteamGetLobbyOwner(int64 lobbyId)
{
	STEAM_CLIENT(0);
	CSteamID ownerId = SteamMatchmaking()->GetLobbyOwner(INT64_TO_STEAMID(lobbyId));
	return STEAMID_TO_INT64(ownerId);
}

bool UAPIMatchmakingClient::SteamIsLobbyOwner(int64 lobbyId)
{
	CSteamID me = SteamUser()->GetSteamID();
	CSteamID owner = SteamMatchmaking()->GetLobbyOwner(INT64_TO_STEAMID(lobbyId));
	return me == owner;
}

int32 UAPIMatchmakingClient::SteamGetNumLobbyMembers(int64 lobbyId)
{
	STEAM_CLIENT(0);
	return SteamMatchmaking()->GetNumLobbyMembers(INT64_TO_STEAMID(lobbyId));
}

TArray<int64> UAPIMatchmakingClient::SteamGetLobbyMembers(int64 lobbyId, bool includeLocalUser)
{
	TArray<int64> result = TArray<int64>();
	STEAM_CLIENT(result);

	CSteamID lobby = INT64_TO_STEAMID(lobbyId);
	CSteamID me = SteamUser()->GetSteamID();
	int count = SteamMatchmaking()->GetNumLobbyMembers(lobby);
	for (int i = 0; i < count; i++)
	{
		CSteamID target = SteamMatchmaking()->GetLobbyMemberByIndex(lobby, i);
		if (includeLocalUser
			|| target != me)
			result.Add(STEAMID_TO_INT64(target));
	}

	return result;
}

bool UAPIMatchmakingClient::SteamInviteUserToLobby(int64 lobbyId, int64 userId)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->InviteUserToLobby(INT64_TO_STEAMID(lobbyId), INT64_TO_STEAMID(userId));
}

void UAPIMatchmakingClient::SteamJoinLobby(int64 lobbyId, FLobbyEnter callback)
{
	STEAM_CLIENT(;);
	FLobbyEnterLinker* linker = new FLobbyEnterLinker(lobbyId, callback);
}

void UAPIMatchmakingClient::SteamLeaveLobby(int64 lobbyId)
{
	STEAM_CLIENT(;);
	SteamMatchmaking()->LeaveLobby(INT64_TO_STEAMID(lobbyId));
}

void UAPIMatchmakingClient::SteamQuickMatch(FLobbyMatch callback)
{
	STEAM_CLIENT(;);
	FLobbyQuickMatchLinker* linker = new FLobbyQuickMatchLinker(callback);
}

bool UAPIMatchmakingClient::SteamRemoveFavoriteGame(int32 appId, FString ip, int32 connectionPort, int32 queryPort)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->RemoveFavoriteGame(static_cast<AppId_t>(appId), HeathenTools::IPAddress(ip), static_cast<uint16>(connectionPort), static_cast<uint16>(queryPort), 0x01);
}

bool UAPIMatchmakingClient::SteamRemoveHistoryGame(int32 appId, FString ip, int32 connectionPort, int32 queryPort)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->RemoveFavoriteGame(static_cast<AppId_t>(appId), HeathenTools::IPAddress(ip), static_cast<uint16>(connectionPort), static_cast<uint16>(queryPort), 0x02);
}

bool UAPIMatchmakingClient::SteamRequestLobbyData(int64 lobbyId)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->RequestLobbyData(INT64_TO_STEAMID(lobbyId));
}

void UAPIMatchmakingClient::SteamRequestLobbyList(FLobbyMatchList callback)
{
	STEAM_CLIENT(;);
	FLobbyMatchListLinker* linker = new FLobbyMatchListLinker(callback);
}

bool UAPIMatchmakingClient::SteamSendLobbyChatData(int64 lobbyId, TArray<uint8> data)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->SendLobbyChatMsg(INT64_TO_STEAMID(lobbyId), (const void*)data.GetData(), data.Num());
}

bool UAPIMatchmakingClient::SteamSendLobbyChatMessage(int64 lobbyId, FString message)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->SendLobbyChatMsg(INT64_TO_STEAMID(lobbyId), (const void*)message.GetCharArray().GetData(), message.Len());
}

bool UAPIMatchmakingClient::SteamSetLobbyData(int64 lobbyId, FString key, FString value)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->SetLobbyData(INT64_TO_STEAMID(lobbyId), StringCast<ANSICHAR>(*key).Get(), StringCast<ANSICHAR>(*value).Get());
}

void UAPIMatchmakingClient::SteamSetLobbyGameHost(int64 lobbyId)
{
	STEAM_CLIENT(;);
	SteamMatchmaking()->SetLobbyGameServer(INT64_TO_STEAMID(lobbyId), 0, 0, SteamUser()->GetSteamID());
}

void UAPIMatchmakingClient::SteamSetLobbyGameServerId(int64 lobbyId, int64 serverId)
{
	STEAM_CLIENT(;);
	SteamMatchmaking()->SetLobbyGameServer(INT64_TO_STEAMID(lobbyId), 0, 0, INT64_TO_STEAMID(serverId));
}

void UAPIMatchmakingClient::SteamSetLobbyGameServerAddress(int64 lobbyId, FString ip, int32 port)
{
	STEAM_CLIENT(;);
	SteamMatchmaking()->SetLobbyGameServer(INT64_TO_STEAMID(lobbyId), HeathenTools::IPAddress(ip), static_cast<uint16>(port), CSteamID());
}

void UAPIMatchmakingClient::SteamSetLobbyGameServer(int64 lobbyId, int64 serverId, FString ip, int32 port)
{
	STEAM_CLIENT(;);
	SteamMatchmaking()->SetLobbyGameServer(INT64_TO_STEAMID(lobbyId), HeathenTools::IPAddress(ip), static_cast<uint16>(port), INT64_TO_STEAMID(serverId));
}

bool UAPIMatchmakingClient::SteamSetLobbyJoinable(int64 lobbyId, bool joinable)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->SetLobbyJoinable(INT64_TO_STEAMID(lobbyId), joinable);
}

void UAPIMatchmakingClient::SteamSetLobbyMemberData(int64 lobbyId, FString key, FString value)
{
	STEAM_CLIENT(;);
	SteamMatchmaking()->SetLobbyMemberData(INT64_TO_STEAMID(lobbyId), StringCast<ANSICHAR>(*key).Get(), StringCast<ANSICHAR>(*value).Get());
}

bool UAPIMatchmakingClient::SteamSetLobbyMemberLimit(int64 lobbyId, int32 maxMembers)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->SetLobbyMemberLimit(INT64_TO_STEAMID(lobbyId), maxMembers);
}

bool UAPIMatchmakingClient::SteamSetLobbyOwner(int64 lobbyId, int64 userId)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->SetLobbyOwner(INT64_TO_STEAMID(lobbyId), INT64_TO_STEAMID(userId));
}

bool UAPIMatchmakingClient::SteamSetLobbyType(int64 lobbyId, UELobbyType type)
{
	STEAM_CLIENT(false);
	return SteamMatchmaking()->SetLobbyType(INT64_TO_STEAMID(lobbyId), static_cast<ELobbyType>(type));
}


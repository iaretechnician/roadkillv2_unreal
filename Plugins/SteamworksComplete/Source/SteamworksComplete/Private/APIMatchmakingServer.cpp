/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "APIMatchmakingServer.h"

void UAPIMatchmakingServer::SteamServerPingServer(FString address, int32 port, FSteamMatchmakingPingResponse callback)
{
	STEAM_CLIENT(;);
	SteamMatchmakingPingResponseLinker* linker = new SteamMatchmakingPingResponseLinker(address, port, callback);
}

void UAPIMatchmakingServer::SteamServerPlayerDetails(FString address, int32 port, FSteamMatchmakingPlayersResponse callback)
{
	STEAM_CLIENT(;);
	SteamMatchmakingPlayersResponseLinker* linker = new SteamMatchmakingPlayersResponseLinker(address, port, callback);
}

void UAPIMatchmakingServer::SteamServerRequestFavoritesServerList(int32 appId, TArray<FKeyValuePair> filter, FSteamServerListResponce callback)
{
	STEAM_CLIENT(;);

	if (filter.Num() > 0)
	{
		TArray<MatchMakingKeyValuePair_t> pFilter;
		pFilter.Reserve(filter.Num());
		for (int i = 0; i < filter.Num(); i++)
		{
			MatchMakingKeyValuePair_t nEntry(StringCast<ANSICHAR>(*filter[i].key).Get(), StringCast<ANSICHAR>(*filter[i].value).Get());
			pFilter.Add(nEntry);
		}

		// Get a pointer to the data in the vector
		MatchMakingKeyValuePair_t* pFilterArray = pFilter.GetData();
		uint32 nFilters = pFilter.Num();

		SteamMatchmakingServerListResponseLinker* linker = new SteamMatchmakingServerListResponseLinker(callback);
		linker->listHandle = SteamMatchmakingServers()->RequestFavoritesServerList(static_cast<AppId_t>(appId), &pFilterArray, nFilters, linker);
	}
	else
	{
		SteamMatchmakingServerListResponseLinker* linker = new SteamMatchmakingServerListResponseLinker(callback);
		linker->listHandle = SteamMatchmakingServers()->RequestFavoritesServerList(static_cast<AppId_t>(appId), NULL, 0, linker);
	}
}

void UAPIMatchmakingServer::SteamServerRequestFriendsServerList(int32 appId, TArray<FKeyValuePair> filter, FSteamServerListResponce callback)
{
	STEAM_CLIENT(;);

	if (filter.Num() > 0)
	{
		TArray<MatchMakingKeyValuePair_t> pFilter;
		pFilter.Reserve(filter.Num());
		for (int i = 0; i < filter.Num(); i++)
		{
			MatchMakingKeyValuePair_t nEntry(StringCast<ANSICHAR>(*filter[i].key).Get(), StringCast<ANSICHAR>(*filter[i].value).Get());
			pFilter.Add(nEntry);
		}

		// Get a pointer to the data in the vector
		MatchMakingKeyValuePair_t* pFilterArray = pFilter.GetData();
		uint32 nFilters = pFilter.Num();

		SteamMatchmakingServerListResponseLinker* linker = new SteamMatchmakingServerListResponseLinker(callback);
		linker->listHandle = SteamMatchmakingServers()->RequestFriendsServerList(static_cast<AppId_t>(appId), &pFilterArray, nFilters, linker);
	}
	else
	{
		SteamMatchmakingServerListResponseLinker* linker = new SteamMatchmakingServerListResponseLinker(callback);
		linker->listHandle = SteamMatchmakingServers()->RequestFriendsServerList(static_cast<AppId_t>(appId), NULL, 0, linker);
	}
}

void UAPIMatchmakingServer::SteamServerRequestHistoryServerList(int32 appId, TArray<FKeyValuePair> filter, FSteamServerListResponce callback)
{
	STEAM_CLIENT(;);

	if (filter.Num() > 0)
	{
		TArray<MatchMakingKeyValuePair_t> pFilter;
		pFilter.Reserve(filter.Num());
		for (int i = 0; i < filter.Num(); i++)
		{
			MatchMakingKeyValuePair_t nEntry(StringCast<ANSICHAR>(*filter[i].key).Get(), StringCast<ANSICHAR>(*filter[i].value).Get());
			pFilter.Add(nEntry);
		}

		// Get a pointer to the data in the vector
		MatchMakingKeyValuePair_t* pFilterArray = pFilter.GetData();
		uint32 nFilters = pFilter.Num();

		SteamMatchmakingServerListResponseLinker* linker = new SteamMatchmakingServerListResponseLinker(callback);
		linker->listHandle = SteamMatchmakingServers()->RequestHistoryServerList(static_cast<AppId_t>(appId), &pFilterArray, nFilters, linker);
	}
	else
	{
		SteamMatchmakingServerListResponseLinker* linker = new SteamMatchmakingServerListResponseLinker(callback);
		linker->listHandle = SteamMatchmakingServers()->RequestHistoryServerList(static_cast<AppId_t>(appId), NULL, 0, linker);
	}
}

void UAPIMatchmakingServer::SteamServerRequestInternetServerList(int32 appId, TArray<FKeyValuePair> filter, FSteamServerListResponce callback)
{
	STEAM_CLIENT(;);

	if (filter.Num() > 0)
	{
		TArray<MatchMakingKeyValuePair_t> pFilter;
		pFilter.Reserve(filter.Num());
		for (int i = 0; i < filter.Num(); i++)
		{
			MatchMakingKeyValuePair_t nEntry(StringCast<ANSICHAR>(*filter[i].key).Get(), StringCast<ANSICHAR>(*filter[i].value).Get());
			pFilter.Add(nEntry);
		}

		// Get a pointer to the data in the vector
		MatchMakingKeyValuePair_t* pFilterArray = pFilter.GetData();
		uint32 nFilters = pFilter.Num();

		SteamMatchmakingServerListResponseLinker* linker = new SteamMatchmakingServerListResponseLinker(callback);
		linker->listHandle = SteamMatchmakingServers()->RequestInternetServerList(static_cast<AppId_t>(appId), &pFilterArray, nFilters, linker);
	}
	else
	{
		SteamMatchmakingServerListResponseLinker* linker = new SteamMatchmakingServerListResponseLinker(callback);
		linker->listHandle = SteamMatchmakingServers()->RequestInternetServerList(static_cast<AppId_t>(appId), NULL, 0, linker);
	}
}

void UAPIMatchmakingServer::SteamServerRequestLANServerList(int32 appId, FSteamServerListResponce callback)
{
	STEAM_CLIENT(;);


	SteamMatchmakingServerListResponseLinker* linker = new SteamMatchmakingServerListResponseLinker(callback);
	linker->listHandle = SteamMatchmakingServers()->RequestLANServerList(static_cast<AppId_t>(appId), linker);
}

void UAPIMatchmakingServer::SteamServerRequestSpectatorServerList(int32 appId, TArray<FKeyValuePair> filter, FSteamServerListResponce callback)
{
	STEAM_CLIENT(;);

	if (filter.Num() > 0)
	{
		TArray<MatchMakingKeyValuePair_t> pFilter;
		pFilter.Reserve(filter.Num());
		for (int i = 0; i < filter.Num(); i++)
		{
			MatchMakingKeyValuePair_t nEntry(StringCast<ANSICHAR>(*filter[i].key).Get(), StringCast<ANSICHAR>(*filter[i].value).Get());
			pFilter.Add(nEntry);
		}

		// Get a pointer to the data in the vector
		MatchMakingKeyValuePair_t *pFilterArray = pFilter.GetData();
		uint32 nFilters = pFilter.Num();

		SteamMatchmakingServerListResponseLinker* linker = new SteamMatchmakingServerListResponseLinker(callback);
		linker->listHandle = SteamMatchmakingServers()->RequestSpectatorServerList(static_cast<AppId_t>(appId), &pFilterArray, nFilters, linker);
	}
	else
	{
		SteamMatchmakingServerListResponseLinker* linker = new SteamMatchmakingServerListResponseLinker(callback);
		linker->listHandle = SteamMatchmakingServers()->RequestSpectatorServerList(static_cast<AppId_t>(appId), NULL, 0, linker);
	}
}

void UAPIMatchmakingServer::SteamServerRules(FString address, int32 port, FSteamMatchmakingRulesResponse callback)
{
	STEAM_CLIENT(;);
	SteamMatchmakingRulesResponseLinker* linker = new SteamMatchmakingRulesResponseLinker(address, port, callback);
}
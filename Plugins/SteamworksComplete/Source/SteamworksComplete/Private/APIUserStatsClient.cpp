/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "APIUserStatsClient.h"

void UAPIUserStatsClient::SteamAttachLeaderboardUGC(int64 boardId, int64 ugcHandle, FLeaderboardUGCSet callback)
{
	STEAM_CLIENT(;);
	SteamLeaderboard_t board_t = static_cast<SteamLeaderboard_t>(boardId);
	UGCHandle_t ugc_t = static_cast<UGCHandle_t>(ugcHandle);
	APIUserStatsClientLeaderboardUGCSetLinker* linker = new APIUserStatsClientLeaderboardUGCSetLinker(board_t, ugc_t, callback);
}

bool UAPIUserStatsClient::SteamClearAchievement(FString achievementApiName)
{
	STEAM_CLIENT(false);
	bool result = SteamUserStats()->ClearAchievement(StringCast<ANSICHAR>(*achievementApiName).Get());
	return result;
}

void UAPIUserStatsClient::SteamDownloadLeaderboardEntries(int64 boardId, UELeaderboardDataRequest requestType, int32 start, int32 end, int32 detailCount, FLeaderboardEntriesDownloaded callback)
{
	STEAM_CLIENT(;);
	SteamLeaderboard_t leaderboard = static_cast<SteamLeaderboard_t>(boardId);
	ELeaderboardDataRequest dRequest = static_cast<ELeaderboardDataRequest>(requestType);
	SteamAPICall_t handle = SteamUserStats()->DownloadLeaderboardEntries(leaderboard, dRequest, start, end);
	LeaderboardDownloadRequestLinker* linker = new LeaderboardDownloadRequestLinker(handle, detailCount, callback);
}

void UAPIUserStatsClient::SteamDownloadLeaderboardEntriesForUsers(int64 boardId, TArray<int64> users, int32 detailCount, FLeaderboardEntriesDownloaded callback)
{
	STEAM_CLIENT(;);
	if (users.GetAllocatedSize() != 0)
	{
		CSteamID* targetUsers = new CSteamID[users.Num()];
		int numUsers = users.Num();
		for (int i = 0; i < numUsers; i++)
		{
			targetUsers[i] = INT64_TO_STEAMID(users[i]);
		}
		SteamLeaderboard_t leaderboard = static_cast<SteamLeaderboard_t>(boardId);
		SteamAPICall_t handle = SteamUserStats()->DownloadLeaderboardEntriesForUsers(leaderboard, targetUsers, numUsers);
		LeaderboardDownloadRequestLinker* linker = new LeaderboardDownloadRequestLinker(handle, detailCount, callback);
	}
}

void UAPIUserStatsClient::SteamFindLeaderboard(FString apiName, FLeaderboardFindResult callback)
{
	STEAM_CLIENT(;);
	APIUserStatsClientLeaderboardFindResultLinker* linker = new APIUserStatsClientLeaderboardFindResultLinker(apiName, callback);
}

void UAPIUserStatsClient::SteamFindOrCreateLeaderboard(FString apiName, UELeaderboardSortMethod sortMethod, UELeaderboardDisplayType displayType, FLeaderboardFindResult callback)
{
	STEAM_CLIENT(;);
	APIUserStatsClientLeaderboardFindOrCreateResultLinker* linker = new APIUserStatsClientLeaderboardFindOrCreateResultLinker(apiName, sortMethod, displayType, callback);
}

bool UAPIUserStatsClient::SteamGetAchievementIsAchieved(FString apiName)
{
	STEAM_CLIENT(false);
	bool achieved;
	SteamUserStats()->GetAchievement(StringCast<ANSICHAR>(*apiName).Get(), &achieved);
	return achieved;
}

FAchievementStatus UAPIUserStatsClient::SteamGetAchievement(FString apiName)
{
	STEAM_CLIENT(FAchievementStatus());
	
	bool achieved;
	uint32 unixTime;
	SteamUserStats()->GetAchievementAndUnlockTime(StringCast<ANSICHAR>(*apiName).Get(), &achieved, &unixTime);

	float pert;
	SteamUserStats()->GetAchievementAchievedPercent(StringCast<ANSICHAR>(*apiName).Get(), &pert);

	FString name = FString(SteamUserStats()->GetAchievementDisplayAttribute(StringCast<ANSICHAR>(*apiName).Get(), "name"));

	FString desc = FString(SteamUserStats()->GetAchievementDisplayAttribute(StringCast<ANSICHAR>(*apiName).Get(), "desc"));

	const char* DisplayAttribute = SteamUserStats()->GetAchievementDisplayAttribute(StringCast<ANSICHAR>(*apiName).Get(), "hidden");
	bool isHidden = (DisplayAttribute && strcmp(DisplayAttribute, "1") == 0);
	delete[] DisplayAttribute;

	FAchievementStatus status;
	status.Achieved = achieved;
	status.Percent = pert;
	status.UnlockTime = FDateTime::FromUnixTimestamp(static_cast<int64>(unixTime));
	status.Name = name;
	status.Description = desc;
	status.IsHidden = isHidden;

	return status;
}

void UAPIUserStatsClient::SteamGetAchievementIcon(FString apiName, FIconLoadCallback callback)
{
	STEAM_CLIENT_INSTANCE(;);
	int32 handle = SteamUserStats()->GetAchievementIcon(StringCast<ANSICHAR>(*apiName).Get());
	if (handle > 0)
		SteamGameInstance->LoadIcon(handle, apiName, callback);
	else
	{
		if (SteamGameInstance->AchievementIconRequests.Contains(apiName))
			SteamGameInstance->AchievementIconRequests.Remove(apiName);

		SteamGameInstance->AchievementIconRequests.Add(apiName, callback);
	}
}

FString UAPIUserStatsClient::SteamGetAchievementApiName(int32 index)
{
	STEAM_CLIENT(FString());
	return FString(SteamUserStats()->GetAchievementName(index));
}

int64 UAPIUserStatsClient::SteamGetGlobalStatInt64(FString apiName)
{
	STEAM_CLIENT(0);
	int64 valueBuffer = 0;
	SteamUserStats()->GetGlobalStat(StringCast<ANSICHAR>(*apiName).Get(), &valueBuffer);
	return valueBuffer;
}

double UAPIUserStatsClient::SteamGetGlobalStatDouble(FString apiName)
{
	STEAM_CLIENT(0.0);
	double valueBuffer = 0;
	SteamUserStats()->GetGlobalStat(StringCast<ANSICHAR>(*apiName).Get(), &valueBuffer);
	return valueBuffer;
}

void UAPIUserStatsClient::SteamGetGlobalStatHistoryInt64(FString apiName, int32 numOfDays, FGlobalStatsIntHistoryReceived callback)
{
	STEAM_CLIENT(;)
	APIUserStatsClientGlobalStatIntLinker* linker = new APIUserStatsClientGlobalStatIntLinker(apiName, numOfDays, callback);
}

void UAPIUserStatsClient::SteamGetGlobalStatHistoryDouble(FString apiName, int32 numOfDays, FGlobalStatsDoubleHistoryReceived callback)
{
	STEAM_CLIENT(;)
	APIUserStatsClientGlobalStatDoubleLinker* linker = new APIUserStatsClientGlobalStatDoubleLinker(apiName, numOfDays, callback);
}

UELeaderboardDisplayType UAPIUserStatsClient::SteamGetLeaderboardDisplayType(int64 boardId)
{
	STEAM_CLIENT(UELeaderboardDisplayType::EPC_None);
	SteamLeaderboard_t id = static_cast<SteamLeaderboard_t>(boardId);
	ELeaderboardDisplayType dType = SteamUserStats()->GetLeaderboardDisplayType(id);
	return static_cast<UELeaderboardDisplayType>(dType);
}

int32 UAPIUserStatsClient::SteamGetLeaderboardEntryCount(int64 boardId)
{
	STEAM_CLIENT(0);
	SteamLeaderboard_t id = static_cast<SteamLeaderboard_t>(boardId);
	return SteamUserStats()->GetLeaderboardEntryCount(id);
}

FString UAPIUserStatsClient::SteamGetLeaderboardName(int64 boardId)
{
	STEAM_CLIENT(FString());
	SteamLeaderboard_t id = static_cast<SteamLeaderboard_t>(boardId);
	return FString(SteamUserStats()->GetLeaderboardName(id));
}

UELeaderboardSortMethod UAPIUserStatsClient::SteamGetLeaderboardSortMethod(int64 boardId)
{
	STEAM_CLIENT(UELeaderboardSortMethod::EPC_None);
	SteamLeaderboard_t id = static_cast<SteamLeaderboard_t>(boardId);
	ELeaderboardSortMethod sort = SteamUserStats()->GetLeaderboardSortMethod(id);
	return static_cast<UELeaderboardSortMethod>(sort);
}

void UAPIUserStatsClient::SteamGetMostAchievedAchievementInfo(FMostAchievedStatusInfo callback)
{
	STEAM_CLIENT(;);
	APIUserStatsClientMostAchievedLinker* linker = new APIUserStatsClientMostAchievedLinker(callback);
}

void UAPIUserStatsClient::SteamGetNumAchievements(FNumCurrentPlayers callback)
{
	STEAM_CLIENT(;);
	APIUserStatsClientNumberOfCurrentPlayersLinker* linker = new APIUserStatsClientNumberOfCurrentPlayersLinker(callback);
}

FIntStat UAPIUserStatsClient::SteamGetIntStat(FString apiName)
{
	STEAM_CLIENT(FIntStat());
	int32 data;

	FIntStat result;
	result.Valid = SteamUserStats()->GetStat(StringCast<ANSICHAR>(*apiName).Get(), &data);
	result.Value = data;
	
	return result;
}

FFloatStat UAPIUserStatsClient::SteamGetFloatStat(FString apiName)
{
	STEAM_CLIENT(FFloatStat());
	float data;

	FFloatStat result;
	result.Valid = SteamUserStats()->GetStat(StringCast<ANSICHAR>(*apiName).Get(), &data);
	result.Value = data;

	return result;
}

FUserAchievementStatus UAPIUserStatsClient::SteamGetUserAchievement(int64 userId, FString apiName)
{
	STEAM_CLIENT(FUserAchievementStatus());
	CSteamID steamId = INT64_TO_STEAMID(userId);

	bool achieved;
	uint32 unixTime;
	SteamUserStats()->GetUserAchievementAndUnlockTime(steamId, StringCast<ANSICHAR>(*apiName).Get(), &achieved, &unixTime);

	FUserAchievementStatus status;
	status.Achieved = achieved;
	status.UnlockTime = FDateTime::FromUnixTimestamp(static_cast<int64>(unixTime));

	return status;
}

FIntStat UAPIUserStatsClient::SteamGetUserIntStat(int64 userId, FString apiName)
{
	STEAM_CLIENT(FIntStat());
	CSteamID steamId = INT64_TO_STEAMID(userId);
	int32 data;

	FIntStat result;
	result.Valid = SteamUserStats()->GetUserStat(steamId, StringCast<ANSICHAR>(*apiName).Get(), &data);
	result.Value = data;

	return result;
}

FFloatStat UAPIUserStatsClient::SteamGetUserFloatStat(int64 userId, FString apiName)
{
	STEAM_CLIENT(FFloatStat());
	CSteamID steamId = INT64_TO_STEAMID(userId);
	float data;

	FFloatStat result;
	result.Valid = SteamUserStats()->GetUserStat(steamId, StringCast<ANSICHAR>(*apiName).Get(), &data);
	result.Value = data;

	return result;
}

bool UAPIUserStatsClient::SteamIndicateAchievementProgress(FString apiName, int32 currentProgress, int32 maxProgress)
{
	STEAM_CLIENT(false);	
	return SteamUserStats()->IndicateAchievementProgress(StringCast<ANSICHAR>(*apiName).Get(), currentProgress, maxProgress);
}

bool UAPIUserStatsClient::SteamRequestCurrentStats()
{
	STEAM_CLIENT(false);
	return SteamUserStats()->RequestCurrentStats();
}

void UAPIUserStatsClient::SteamRequestUserStats(int64 userId, FUserStatsReceivedResult callback)
{
	STEAM_CLIENT(;);
	APIUserStatsClientUserStatsReceivedResultLinker* linker = new APIUserStatsClientUserStatsReceivedResultLinker(INT64_TO_STEAMID(userId), callback);
}

bool UAPIUserStatsClient::SteamResetAllStats(bool achievementsToo)
{
	STEAM_CLIENT(false);
	return SteamUserStats()->ResetAllStats(achievementsToo);
}

bool UAPIUserStatsClient::SteamSetAchievement(FString apiName)
{
	STEAM_CLIENT(false);	
	return SteamUserStats()->SetAchievement(StringCast<ANSICHAR>(*apiName).Get());
}

bool UAPIUserStatsClient::SteamSetIntStat(FString apiName, int32 data)
{
	STEAM_CLIENT(false);
	return SteamUserStats()->SetStat(StringCast<ANSICHAR>(*apiName).Get(), data);
}

bool UAPIUserStatsClient::SteamSetFloatStat(FString apiName, float data)
{
	STEAM_CLIENT(false);
	return SteamUserStats()->SetStat(StringCast<ANSICHAR>(*apiName).Get(), data);
}

bool UAPIUserStatsClient::SteamStoreStats()
{
	STEAM_CLIENT(false);
	return SteamUserStats()->StoreStats();
}

bool UAPIUserStatsClient::SteamUpdateAvgRateStat(FString apiName, float countThisSession, double sessionLength)
{
	STEAM_CLIENT(false);
	return SteamUserStats()->UpdateAvgRateStat(StringCast<ANSICHAR>(*apiName).Get(), countThisSession, sessionLength);
}

void UAPIUserStatsClient::SteamUploadLeaderbaordScore(int64 boardId, bool keepBest, int32 score, TArray<int32> data, FLeaderboardScoreUploaded InCallback)
{
	STEAM_CLIENT(;);
	APIUserStatsClientLeaderboardScoreUploadedLinker* linker = new APIUserStatsClientLeaderboardScoreUploadedLinker(boardId, keepBest, score, data, InCallback);
}

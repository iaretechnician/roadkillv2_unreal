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
#include "APIUserStatsClient.generated.h"

UCLASS()
class STEAMWORKSCOMPLETE_API UAPIUserStatsClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAttachLeaderboardUGC(int64 boardId, int64 ugcHandle, FLeaderboardUGCSet callback);
	
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamClearAchievement(FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamDownloadLeaderboardEntries(int64 boardId, UELeaderboardDataRequest requestType, int32 start, int32 end, int32 detailCount, FLeaderboardEntriesDownloaded callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamDownloadLeaderboardEntriesForUsers(int64 boardId, TArray<int64> users, int32 detailCount, FLeaderboardEntriesDownloaded callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamFindLeaderboard(FString apiName, FLeaderboardFindResult callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamFindOrCreateLeaderboard(FString apiName, UELeaderboardSortMethod sortMethod, UELeaderboardDisplayType displayType, FLeaderboardFindResult callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamGetAchievementIsAchieved(FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FAchievementStatus SteamGetAchievement(FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamGetAchievementIcon(FString apiName, FIconLoadCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetAchievementApiName(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetGlobalStatInt64(FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static double SteamGetGlobalStatDouble(FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamGetGlobalStatHistoryInt64(FString apiName, int32 numOfDays, FGlobalStatsIntHistoryReceived callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamGetGlobalStatHistoryDouble(FString apiName, int32 numOfDays, FGlobalStatsDoubleHistoryReceived callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UELeaderboardDisplayType SteamGetLeaderboardDisplayType(int64 boardId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetLeaderboardEntryCount(int64 boardId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetLeaderboardName(int64 boardId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UELeaderboardSortMethod SteamGetLeaderboardSortMethod(int64 boardId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamGetMostAchievedAchievementInfo(FMostAchievedStatusInfo callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamGetNumAchievements(FNumCurrentPlayers callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FIntStat SteamGetIntStat(FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FFloatStat SteamGetFloatStat(FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FUserAchievementStatus SteamGetUserAchievement(int64 userId, FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FIntStat SteamGetUserIntStat(int64 userId, FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FFloatStat SteamGetUserFloatStat(int64 userId, FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIndicateAchievementProgress(FString apiName, int32 currentProgress, int32 maxProgress);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamRequestCurrentStats();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamRequestUserStats(int64 userId, FUserStatsReceivedResult callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamResetAllStats(bool achievementsToo);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetAchievement(FString apiName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetIntStat(FString apiName, int32 data);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetFloatStat(FString apiName, float data);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamStoreStats();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamUpdateAvgRateStat(FString apiName, float countThisSession, double sessionLength);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamUploadLeaderbaordScore(int64 boardId, bool keepBest, int32 score, TArray<int32> data, FLeaderboardScoreUploaded InCallback);
};

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
#include "APIUGCClient.generated.h"

UCLASS()
class STEAMWORKSCOMPLETE_API UAPIUGCClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddAppDependency(int64 parentFileId, int32 appId, FAddAppDependencyResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddDependency(int64 parentFileId, int64 childFileId, FAddUGCDependencyResultCallback callback); 

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddExcludedTag(int64 queryHandle, FString tagName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddItemKeyValueTag(int64 updateHandle, FString tagName, FString tagValue);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddItemPreviewFile(int64 updateHandle, FString absoluteFilePath, UEItemPreviewType type);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddItemPreviewVideo(int64 updateHandle, FString YouTubeVideoId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddItemToFavorites(int32 appId, int64 fileId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddRequiredTag(int64 queryHandle, FString key);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddRequiredKeyValueTag(int64 queryHandle, FString key, FString value);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAddRequiredTagGroup(int64 queryHandle, TArray<FString> tags);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamCreateItem(int32 appId, UEWorkshopFileType type, FCreateItemResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamCreateQueryAllUGCRequest(UEUGCQuery queryType, UEUGCMatchingUGCType matchingType, int32 createdByAppId, int32 createdForAppId, int32 pageNumber);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamCreateQueryUGCDetailsRequest(TArray<int64> fileIds);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamCreateQueryUserUGCRequest(int32 accountId, UEUserUGCList listType, UEUGCMatchingUGCType matchingType, UEUserUGCListSortOrder sortOrder, int32 creatorAppId, int32 consumerAppId, int32 pageNumber);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamDeleteItem(int64 fileId, FDeleteItemResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamDownloadItem(int64 fileId, bool highPriority);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FUSGCItemDownloadInfo SteamGetItemDownloadInfo(int64 fileId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UEItemState SteamGetItemState(int64 fileId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FUGCItemUpdateProgress SteamGetItemUpdateProgress(int64 updateHandle);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetNumSubscribedItems();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FUGCAdditionalPreviewResult SteamGetQueryUGCAdditionalPreview(int64 queryHandle, int32 index, int32 previewIndex);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FUGCQueryUGCChildrenResult SteamGetQueryUGCChildren(int64 queryHandle, int32 index, int32 numberOfChildren);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetQueryUGCNumTags(int64 queryHandle, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetQueryUGCTag(int64 queryHandle, int32 index, int32 tagIndex);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetQueryUGCTagDisplayName(int64 queryHandle, int32 index, int32 tagIndex);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FKeyValuePair SteamGetQueryUGCKeyValueTag(int64 queryHandle, int32 index, int32 tagIndex);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<UEUGCContentDescriptorID> SteamGetQueryUGCContentDescriptors(int64 queryHandle, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetQueryUGCMetadata(int64 queryHandle, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetQueryUGCNumAdditionalPreviews(int64 queryHandle, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetQueryUGCNumKeyValueTags(int64 queryHandle, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetQueryUGCPreviewURL(int64 queryHandle, int32 index);
	
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FSteamUGCDetails SteamGetQueryUGCResult(int64 queryHandle, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetQueryUGCStatistic(int64 queryHandle, int32 index, UEItemStatistic statistic);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<int64> SteamGetSubscribedItems();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamGetUserItemVote(int64 fileId, FGetUserItemVoteResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamReleaseQueryUGCRequest(int64 queryHandle);
	
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamRemoveAppDependency(int64 fileId, int32 appId, FRemoveAppDependencyResultCallback callback);
	
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void RemoveDependency(int64 parentFileId, int64 childFileId, FRemoveUGCDependencyResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamRemoveItemFromFavorites(int32 appId, int64 fileId, FUserFavoriteItemsListChangedCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamRemoveItemKeyValueTags(int64 updateHandle, FString key);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamRemoveItemPreview(int64 updateHandle, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamAddContentDescriptor(int64 updateHandle, UEUGCContentDescriptorID descriptorId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamRemoveContentDescriptor(int64 updateHandle, UEUGCContentDescriptorID descriptorId);
	
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSendQueryUGCRequest(int64 queryHandle, FSteamUGCQueryCompletedCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetAllowCachedResponse(int64 queryHandle, int32 maxAgeInSeconds);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetCloudFileNameFilter(int64 queryHandle, FString matchCloudFileName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetItemContent(int64 updateHandle, FString absoluteContentFolderPath);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetItemDescription(int64 updateHandle, FString description);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetItemMetadata(int64 updateHandle, FString metadata);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetItemPreview(int64 updateHandle, FString previewFile);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetItemTags(int64 updateHandle, TArray<FString> tags);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetItemTitle(int64 updateHandle, FString title);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetItemUpdateLanguage(int64 updateHandle, FString language);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetItemVisibility(int64 updateHandle, UERemoteStoragePublishedFileVisibility visibility);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetLanguage(int64 queryHandle, FString language);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetMatchAnyTag(int64 queryHandle, bool matchAnyTag);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetRankedByTrendDays(int64 queryHandle, int32 days);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetReturnAdditionalPreviews(int64 queryHandle, bool returnAdditionalPreviews);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetReturnChildren(int64 queryHandle, bool returnChildren);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetReturnKeyValueTags(int64 queryHandle, bool returnKeyValueTags);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetReturnLongDescription(int64 queryHandle, bool returnLongDescription);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetReturnMetadata(int64 queryHandle, bool returnMetadata);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetReturnOnlyIDs(int64 queryHandle, bool returnOnlyIDs);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetReturnPlaytimeStats(int64 queryHandle, int32 Days);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetReturnTotalOnly(int64 queryHandle, bool returnTotalOnly);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetSearchText(int64 queryHandle, FString searchText);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetSearch(int64 queryHandle, FString searchText, bool allowCashedResponse, bool matchAnyTag, bool withAdditionalPreviews, bool withChildren, bool withKeyValueTags, bool withLongDescriptions, bool withMetadata);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSetUserItemVote(int64 fileId, bool voteUp, FSetUserItemVoteResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamStartItemUpdate(int32 appId, int64 fileId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamStartPlaytimeTracking(TArray<int64> fileIDs, FStartPlaytimeTrackingResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamStopPlaytimeTracking(TArray<int64> fileIDs, FStopPlaytimeTrackingResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamStopPlaytimeTrackingForAllItems(FStopPlaytimeTrackingResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSubmitItemUpdate(int64 updateHandle, FString changeNote, FSubmitItemUpdateResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSubscribeItem(int64 fileId, FRemoteStorageSubscribePublishedFileResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSuspendDownloads(bool suspend);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamUnsubscribeItem(int64 fileId, FRemoteStorageUnsubscribePublishedFileResultCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamUpdateItemPreviewFile(int64 updateHandle, int32 index, FString previewFile);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamUpdateItemPreviewVideo(int64 updateHandle, int32 index, FString videoId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamShowWorkshopEULA();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamGetWorkshopEULAStatus(FWorkshopEULAStatusCallback callback);
};

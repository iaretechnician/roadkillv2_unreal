/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "APIUGCClient.h"

void UAPIUGCClient::SteamAddAppDependency(int64 parentFileId, int32 appId, FAddAppDependencyResultCallback callback)
{
	STEAM_CLIENT(;);
	AddAppDependencyResultLinker* linker = new AddAppDependencyResultLinker(parentFileId, appId, callback);
}

void UAPIUGCClient::SteamAddDependency(int64 parentFileId, int64 childFileId, FAddUGCDependencyResultCallback callback)
{
	STEAM_CLIENT(;);
	AddUGCDependencyResultLinker* linker = new AddUGCDependencyResultLinker(parentFileId, childFileId, callback);
}

void UAPIUGCClient::SteamAddExcludedTag(int64 queryHandle, FString tagName)
{
	STEAM_CLIENT(;);
	SteamUGC()->AddExcludedTag(static_cast<UGCQueryHandle_t>(queryHandle), StringCast<ANSICHAR>(*tagName).Get());
}

void UAPIUGCClient::SteamAddItemKeyValueTag(int64 updateHandle, FString tagName, FString tagValue)
{
	STEAM_CLIENT(;);
	SteamUGC()->AddItemKeyValueTag(static_cast<UGCUpdateHandle_t>(updateHandle), StringCast<ANSICHAR>(*tagName).Get(), StringCast<ANSICHAR>(*tagValue).Get());
}

void UAPIUGCClient::SteamAddItemPreviewFile(int64 updateHandle, FString absoluteFilePath, UEItemPreviewType type)
{
	STEAM_CLIENT(;);
	SteamUGC()->AddItemPreviewFile(static_cast<UGCUpdateHandle_t>(updateHandle), StringCast<ANSICHAR>(*absoluteFilePath).Get(), static_cast<EItemPreviewType>(type));
}

void UAPIUGCClient::SteamAddItemPreviewVideo(int64 updateHandle, FString YouTubeVideoId)
{
	STEAM_CLIENT(;);
	SteamUGC()->AddItemPreviewVideo(static_cast<UGCUpdateHandle_t>(updateHandle), StringCast<ANSICHAR>(*YouTubeVideoId).Get());
}

void UAPIUGCClient::SteamAddItemToFavorites(int32 appId, int64 fileId)
{
	STEAM_CLIENT(;);
	SteamUGC()->AddItemToFavorites(static_cast<AppId_t>(appId), static_cast<PublishedFileId_t>(fileId));
}

void UAPIUGCClient::SteamAddRequiredTag(int64 queryHandle, FString key)
{
	STEAM_CLIENT(;);
	SteamUGC()->AddRequiredTag(static_cast<UGCQueryHandle_t>(queryHandle), StringCast<ANSICHAR>(*key).Get());
}

void UAPIUGCClient::SteamAddRequiredKeyValueTag(int64 queryHandle, FString key, FString value)
{
	STEAM_CLIENT(;);
	SteamUGC()->AddRequiredKeyValueTag(static_cast<UGCQueryHandle_t>(queryHandle), StringCast<ANSICHAR>(*key).Get(), StringCast<ANSICHAR>(*value).Get());
}

void UAPIUGCClient::SteamAddRequiredTagGroup(int64 queryHandle, TArray<FString> tags)
{
	STEAM_CLIENT(;);
	const char** stringPointers = new const char* [tags.Num()];
	for (int i = 0; i < tags.Num(); ++i)
	{
		stringPointers[i] = StringCast<ANSICHAR>(*tags[i]).Get();
	}

	SteamParamStringArray_t sArray = SteamParamStringArray_t();
	sArray.m_ppStrings = stringPointers;
	sArray.m_nNumStrings = tags.Num();

	SteamUGC()->AddRequiredTagGroup(static_cast<UGCQueryHandle_t>(queryHandle), &sArray);

	delete[] stringPointers;
}

void UAPIUGCClient::SteamCreateItem(int32 appId, UEWorkshopFileType type, FCreateItemResultCallback callback)
{
	STEAM_CLIENT(;);
	CreateItemResultLinker* linker = new CreateItemResultLinker(appId, type, callback);
}

int64 UAPIUGCClient::SteamCreateQueryAllUGCRequest(UEUGCQuery queryType, UEUGCMatchingUGCType matchingType, int32 createdByAppId, int32 createdForAppId, int32 pageNumber)
{
	STEAM_CLIENT(0);
	UGCQueryHandle_t result = SteamUGC()->CreateQueryAllUGCRequest(static_cast<EUGCQuery>(queryType), static_cast<EUGCMatchingUGCType>(matchingType), static_cast<AppId_t>(createdByAppId), static_cast<AppId_t>(createdForAppId), static_cast<uint32>(pageNumber));
	return static_cast<int64>(result);
}

int64 UAPIUGCClient::SteamCreateQueryUGCDetailsRequest(TArray<int64> fileIds)
{
	STEAM_CLIENT(0);
	PublishedFileId_t* pubIds = new PublishedFileId_t[fileIds.Num()];
	for (int i = 0; i < fileIds.Num(); ++i)
	{
		pubIds[i] = static_cast<PublishedFileId_t>(fileIds[i]);
	}

	UGCQueryHandle_t handle = SteamUGC()->CreateQueryUGCDetailsRequest(pubIds, fileIds.Num());
	delete[] pubIds;
	return static_cast<int64>(handle);
}

int64 UAPIUGCClient::SteamCreateQueryUserUGCRequest(int32 accountId, UEUserUGCList listType, UEUGCMatchingUGCType matchingType, UEUserUGCListSortOrder sortOrder, int32 creatorAppId, int32 consumerAppId, int32 pageNumber)
{
	STEAM_CLIENT(0);
	UGCQueryHandle_t handle = SteamUGC()->CreateQueryUserUGCRequest(static_cast<AccountID_t>(accountId), static_cast<EUserUGCList>(listType), static_cast<EUGCMatchingUGCType>(matchingType), static_cast<EUserUGCListSortOrder>(sortOrder), static_cast<AppId_t>(creatorAppId), static_cast<AppId_t>(consumerAppId), static_cast<uint32>(pageNumber));
	return static_cast<int64>(handle);
}

void UAPIUGCClient::SteamDeleteItem(int64 fileId, FDeleteItemResultCallback callback)
{
	STEAM_CLIENT(;);
	DeleteItemResultLinker* link = new DeleteItemResultLinker(fileId, callback);
}

bool UAPIUGCClient::SteamDownloadItem(int64 fileId, bool highPriority)
{
	STEAM_CLIENT(false);
	return SteamUGC()->DownloadItem(static_cast<PublishedFileId_t>(fileId), highPriority);
}

FUSGCItemDownloadInfo UAPIUGCClient::SteamGetItemDownloadInfo(int64 fileId)
{
	FUSGCItemDownloadInfo result = FUSGCItemDownloadInfo();
	STEAM_CLIENT(result);
	uint64 down;
	uint64 total;
	result.requestResult = SteamUGC()->GetItemDownloadInfo(static_cast<PublishedFileId_t>(fileId), &down, &total);
	result.downloaded = static_cast<int64>(down);
	result.total = static_cast<int64>(total);
	return result;
}

UEItemState UAPIUGCClient::SteamGetItemState(int64 fileId)
{
	STEAM_CLIENT(UEItemState::EPC_None);
	uint32 flags = SteamUGC()->GetItemState(static_cast<PublishedFileId_t>(fileId));
	return static_cast<UEItemState>(flags);
}

FUGCItemUpdateProgress UAPIUGCClient::SteamGetItemUpdateProgress(int64 updateHandle)
{
	FUGCItemUpdateProgress result = FUGCItemUpdateProgress();
	STEAM_CLIENT(result);
	uint64 proc;
	uint64 tot;
	EItemUpdateStatus ret = SteamUGC()->GetItemUpdateProgress(static_cast<UGCHandle_t>(updateHandle), &proc, &tot);
	result.status = static_cast<UEItemUpdateStatus>(ret);
	result.processed = static_cast<int64>(proc);
	result.total = static_cast<int64>(tot);
	return result;
}

int32 UAPIUGCClient::SteamGetNumSubscribedItems()
{
	STEAM_CLIENT(0);
	return static_cast<int32>(SteamUGC()->GetNumSubscribedItems());
}

FUGCAdditionalPreviewResult UAPIUGCClient::SteamGetQueryUGCAdditionalPreview(int64 queryHandle, int32 index, int32 previewIndex)
{
	FUGCAdditionalPreviewResult result = FUGCAdditionalPreviewResult();
	STEAM_CLIENT(result);
	char* videoId = new char[128];
	char* originalfileName = new char[1024];
	EItemPreviewType preType;
	result.requestResult = SteamUGC()->GetQueryUGCAdditionalPreview(static_cast<UGCQueryHandle_t>(queryHandle)
		, static_cast<uint32>(index)
		, static_cast<uint32>(previewIndex)
		, videoId
		, 128
		, originalfileName
		, 1024
		, &preType);

	result.originalFileName = FString(originalfileName);
	result.urlOrVideoID = FString(videoId);
	result.type = static_cast<UEItemPreviewType>(preType);

	return result;
}

FUGCQueryUGCChildrenResult UAPIUGCClient::SteamGetQueryUGCChildren(int64 queryHandle, int32 index, int32 numberOfChildren)
{
	FUGCQueryUGCChildrenResult result = FUGCQueryUGCChildrenResult();
	STEAM_CLIENT(result);
	PublishedFileId_t* fileIds = new PublishedFileId_t[numberOfChildren];
	result.requestStatus = SteamUGC()->GetQueryUGCChildren(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index), fileIds, static_cast<uint32>(numberOfChildren));
	for (int i = 0; i < numberOfChildren; i++)
	{
		result.fileIDs.Add(static_cast<int64>(fileIds[i]));
	}
	delete[] fileIds;
	return result;
}

int32 UAPIUGCClient::SteamGetQueryUGCNumTags(int64 queryHandle, int32 index)
{
	STEAM_CLIENT(0);
	uint32 result = SteamUGC()->GetQueryUGCNumTags(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index));
	return static_cast<int32>(result);
}

FString UAPIUGCClient::SteamGetQueryUGCTag(int64 queryHandle, int32 index, int32 tagIndex)
{
	STEAM_CLIENT(FString());

	char* value = new char[128];
	SteamUGC()->GetQueryUGCTag(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index), static_cast<uint32>(tagIndex), value, 128);
	FString result = FString(value);
	return result;
}

FString UAPIUGCClient::SteamGetQueryUGCTagDisplayName(int64 queryHandle, int32 index, int32 tagIndex)
{
	STEAM_CLIENT(FString());

	char* value = new char[256];
	SteamUGC()->GetQueryUGCTagDisplayName(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index), static_cast<uint32>(tagIndex), value, 256);
	FString result = FString(value);
	return result;
}

FKeyValuePair UAPIUGCClient::SteamGetQueryUGCKeyValueTag(int64 queryHandle, int32 index, int32 tagIndex)
{
	FKeyValuePair result = FKeyValuePair();
	STEAM_CLIENT(result);

	char* key = new char[128];
	char* value = new char[128];
	SteamUGC()->GetQueryUGCKeyValueTag(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index), static_cast<uint32>(tagIndex), key, 128, value, 128);
	result.key = FString(key);
	result.value = FString(value);

	return result;
}

TArray<UEUGCContentDescriptorID> UAPIUGCClient::SteamGetQueryUGCContentDescriptors(int64 queryHandle, int32 index)
{
	TArray<UEUGCContentDescriptorID> result;
	STEAM_CLIENT(result);
	EUGCContentDescriptorID* pDec = new EUGCContentDescriptorID[5];
	uint32 ret = SteamUGC()->GetQueryUGCContentDescriptors(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index), pDec, 5);
	
	if (ret > 0)
	{
		for (uint32 i = 0; i < ret; i++)
		{
			result.Add(static_cast<UEUGCContentDescriptorID>(pDec[i]));
		}
	}
	
	delete[] pDec;

	return result;
}

FString UAPIUGCClient::SteamGetQueryUGCMetadata(int64 queryHandle, int32 index)
{
	STEAM_CLIENT(FString());
	
	char* value = new char[k_cchDeveloperMetadataMax];
	SteamUGC()->GetQueryUGCMetadata(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index), value, k_cchDeveloperMetadataMax);
	FString result = FString(value);
	return result;
}

int32 UAPIUGCClient::SteamGetQueryUGCNumAdditionalPreviews(int64 queryHandle, int32 index)
{
	STEAM_CLIENT(0);
	uint32 result = SteamUGC()->GetQueryUGCNumAdditionalPreviews(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index));
	return static_cast<int32>(result);
}

int32 UAPIUGCClient::SteamGetQueryUGCNumKeyValueTags(int64 queryHandle, int32 index)
{
	STEAM_CLIENT(0);
	uint32 result = SteamUGC()->GetQueryUGCNumKeyValueTags(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index));
	return static_cast<int32>(result);
}

FString UAPIUGCClient::SteamGetQueryUGCPreviewURL(int64 queryHandle, int32 index)
{
	STEAM_CLIENT(FString());
	char* value = new char[1024];
	SteamUGC()->GetQueryUGCPreviewURL(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index), value, 1024);
	FString result = FString(value);
	return result;
}

FSteamUGCDetails UAPIUGCClient::SteamGetQueryUGCResult(int64 queryHandle, int32 index)
{
	FSteamUGCDetails result = FSteamUGCDetails();
	STEAM_CLIENT(result);
	SteamUGCDetails_t details;
	if (SteamUGC()->GetQueryUGCResult(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index), &details))
	{
		result.acceptedForUse = details.m_bAcceptedForUse;
		result.banned = details.m_bBanned;
		result.consumerAppId = static_cast<int32>(details.m_nConsumerAppID);
		result.creatorAppId = static_cast<int32>(details.m_nCreatorAppID);
		result.description = FString(details.m_rgchDescription);
		result.fileHandle = static_cast<int64>(details.m_hFile);
		result.fileId = static_cast<int64>(details.m_nPublishedFileId);
		result.fileName = FString(details.m_pchFileName);
		result.fileSize = details.m_nFileSize;
		result.fileType = static_cast<UEWorkshopFileType>(details.m_eFileType);
		result.numberOfChildren = static_cast<int32>(details.m_unNumChildren);
		result.ownerId = static_cast<int64>(details.m_ulSteamIDOwner);
		result.previewFileHandle = static_cast<int64>(details.m_hPreviewFile);
		result.previewFileSize = details.m_nPreviewFileSize;
		result.result = static_cast<UEResult>(details.m_eResult);
		result.score = details.m_flScore;

		FString tagString = FString(details.m_rgchTags);
		tagString.ParseIntoArray(result.tags, TEXT(","), true);

		result.tagsTruncated = details.m_bTagsTruncated;
		result.timeAddedToUserList = FDateTime::FromUnixTimestamp(static_cast<int64>(details.m_rtimeAddedToUserList));
		result.timeCreated = FDateTime::FromUnixTimestamp(static_cast<int64>(details.m_rtimeCreated));
		result.timeUpdated = FDateTime::FromUnixTimestamp(static_cast<int64>(details.m_rtimeUpdated));
		result.title = FString(details.m_rgchTitle);
		result.url = FString(details.m_rgchURL);
		result.visibility = static_cast<UERemoteStoragePublishedFileVisibility>(details.m_eVisibility);
		result.votesDown = static_cast<int32>(details.m_unVotesDown);
		result.votesUp = static_cast<int32>(details.m_unVotesUp);
	}
	else
		result.result = UEResult::EPC_Fail;

	return result;
}

int64 UAPIUGCClient::SteamGetQueryUGCStatistic(int64 queryHandle, int32 index, UEItemStatistic statistic)
{
	STEAM_CLIENT(0);
	uint64 val;
	SteamUGC()->GetQueryUGCStatistic(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(index), static_cast<EItemStatistic>(statistic), &val);
	return static_cast<int64>(val);
}

TArray<int64> UAPIUGCClient::SteamGetSubscribedItems()
{
	TArray<int64> ret = TArray<int64>();

	STEAM_CLIENT(ret);
	
	uint32 count = SteamUGC()->GetNumSubscribedItems();
	PublishedFileId_t* buffer = new PublishedFileId_t[count];
	uint64 result = SteamUGC()->GetSubscribedItems(buffer, count);

	for (int i = 0; i < result; i++)
	{
		ret.Add(static_cast<int64>(buffer[i]));
	}

	delete[] buffer;

	return ret;
}

void UAPIUGCClient::SteamGetUserItemVote(int64 fileId, FGetUserItemVoteResultCallback callback)
{
	STEAM_CLIENT(;);
	GetUserItemVoteResultLinker* linker = new GetUserItemVoteResultLinker(fileId, callback);
}

void UAPIUGCClient::SteamReleaseQueryUGCRequest(int64 queryHandle)
{
	STEAM_CLIENT(;);
	SteamUGC()->ReleaseQueryUGCRequest(static_cast<UGCQueryHandle_t>(queryHandle));
}

void UAPIUGCClient::SteamRemoveAppDependency(int64 fileId, int32 appId, FRemoveAppDependencyResultCallback callback)
{
	STEAM_CLIENT(;);
	RemoveAppDependencyResultLinker* linker = new RemoveAppDependencyResultLinker(fileId, appId, callback);
}

void UAPIUGCClient::RemoveDependency(int64 parentFileId, int64 childFileId, FRemoveUGCDependencyResultCallback callback)
{
	STEAM_CLIENT(;);
	RemoveUGCDependencyResultLinker* linker = new RemoveUGCDependencyResultLinker(parentFileId, childFileId, callback);
}

void UAPIUGCClient::SteamRemoveItemFromFavorites(int32 appId, int64 fileId, FUserFavoriteItemsListChangedCallback callback)
{
	STEAM_CLIENT(;);
	UserFavoriteItemsListChangedLinker* linker = new UserFavoriteItemsListChangedLinker(appId, fileId, callback);
}

bool UAPIUGCClient::SteamRemoveItemKeyValueTags(int64 updateHandle, FString key)
{
	STEAM_CLIENT(false);
	return SteamUGC()->RemoveItemKeyValueTags(static_cast<UGCUpdateHandle_t>(updateHandle), StringCast<ANSICHAR>(*key).Get());
}

bool UAPIUGCClient::SteamRemoveItemPreview(int64 updateHandle, int32 index)
{
	STEAM_CLIENT(false);
	return SteamUGC()->RemoveItemPreview(static_cast<UGCUpdateHandle_t>(updateHandle), static_cast<uint32>(index));
}

bool UAPIUGCClient::SteamAddContentDescriptor(int64 updateHandle, UEUGCContentDescriptorID descriptorId)
{
	STEAM_CLIENT(false);
	return SteamUGC()->AddContentDescriptor(static_cast<UGCUpdateHandle_t>(updateHandle), static_cast<EUGCContentDescriptorID>(descriptorId));
}

bool UAPIUGCClient::SteamRemoveContentDescriptor(int64 updateHandle, UEUGCContentDescriptorID descriptorId)
{
	STEAM_CLIENT(false);
	return SteamUGC()->RemoveContentDescriptor(static_cast<UGCUpdateHandle_t>(updateHandle), static_cast<EUGCContentDescriptorID>(descriptorId));
}

void UAPIUGCClient::SteamSendQueryUGCRequest(int64 queryHandle, FSteamUGCQueryCompletedCallback callback)
{
	STEAM_CLIENT(;);
	SteamUGCQueryCompletedLinker* linker = new SteamUGCQueryCompletedLinker(queryHandle, callback);
}

bool UAPIUGCClient::SteamSetAllowCachedResponse(int64 queryHandle, int32 maxAgeInSeconds)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetAllowCachedResponse(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(maxAgeInSeconds));
}

bool UAPIUGCClient::SteamSetCloudFileNameFilter(int64 queryHandle, FString matchCloudFileName)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetCloudFileNameFilter(static_cast<UGCQueryHandle_t>(queryHandle), StringCast<ANSICHAR>(*matchCloudFileName).Get());
}

bool UAPIUGCClient::SteamSetItemContent(int64 updateHandle, FString absoluteContentFolderPath)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetItemContent(static_cast<UGCUpdateHandle_t>(updateHandle), StringCast<ANSICHAR>(*absoluteContentFolderPath).Get());
}

bool UAPIUGCClient::SteamSetItemDescription(int64 updateHandle, FString description)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetItemDescription(static_cast<UGCUpdateHandle_t>(updateHandle), StringCast<ANSICHAR>(*description).Get());
}

bool UAPIUGCClient::SteamSetItemMetadata(int64 updateHandle, FString metadata)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetItemMetadata(static_cast<UGCUpdateHandle_t>(updateHandle), StringCast<ANSICHAR>(*metadata).Get());
}

bool UAPIUGCClient::SteamSetItemPreview(int64 updateHandle, FString previewFile)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetItemPreview(static_cast<UGCUpdateHandle_t>(updateHandle), StringCast<ANSICHAR>(*previewFile).Get());
}

bool UAPIUGCClient::SteamSetItemTags(int64 updateHandle, TArray<FString> tags)
{
	STEAM_CLIENT(false);

	TArray<const char*> CharPtrArray;
	for (const FString& Tag : tags)
	{
		CharPtrArray.Add(StringCast<ANSICHAR>(*Tag).Get());
	}

	SteamParamStringArray_t sTags{};
	sTags.m_ppStrings = CharPtrArray.GetData();
	sTags.m_nNumStrings = CharPtrArray.Num();

	return SteamUGC()->SetItemTags(static_cast<UGCUpdateHandle_t>(updateHandle), &sTags);
}

bool UAPIUGCClient::SteamSetItemTitle(int64 updateHandle, FString title)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetItemTitle(static_cast<UGCUpdateHandle_t>(updateHandle), StringCast<ANSICHAR>(*title).Get());
}

bool UAPIUGCClient::SteamSetItemUpdateLanguage(int64 updateHandle, FString language)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetItemUpdateLanguage(static_cast<UGCUpdateHandle_t>(updateHandle), StringCast<ANSICHAR>(*language).Get());
}

bool UAPIUGCClient::SteamSetItemVisibility(int64 updateHandle, UERemoteStoragePublishedFileVisibility visibility)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetItemVisibility(static_cast<UGCUpdateHandle_t>(updateHandle), static_cast<ERemoteStoragePublishedFileVisibility>(visibility));
}

bool UAPIUGCClient::SteamSetLanguage(int64 queryHandle, FString language)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetLanguage(static_cast<UGCQueryHandle_t>(queryHandle), StringCast<ANSICHAR>(*language).Get());
}

bool UAPIUGCClient::SteamSetMatchAnyTag(int64 queryHandle, bool matchAnyTag)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetMatchAnyTag(static_cast<UGCQueryHandle_t>(queryHandle), matchAnyTag);
}

bool UAPIUGCClient::SteamSetRankedByTrendDays(int64 queryHandle, int32 days)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetRankedByTrendDays(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(days));
}

bool UAPIUGCClient::SteamSetReturnAdditionalPreviews(int64 queryHandle, bool returnAdditionalPreviews)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetReturnAdditionalPreviews(static_cast<UGCQueryHandle_t>(queryHandle), returnAdditionalPreviews);
}

bool UAPIUGCClient::SteamSetReturnChildren(int64 queryHandle, bool returnChildren)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetReturnChildren(static_cast<UGCQueryHandle_t>(queryHandle), returnChildren);
}

bool UAPIUGCClient::SteamSetReturnKeyValueTags(int64 queryHandle, bool returnKeyValueTags)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetReturnKeyValueTags(static_cast<UGCQueryHandle_t>(queryHandle), returnKeyValueTags);
}

bool UAPIUGCClient::SteamSetReturnLongDescription(int64 queryHandle, bool returnLongDescription)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetReturnLongDescription(static_cast<UGCQueryHandle_t>(queryHandle), returnLongDescription);
}

bool UAPIUGCClient::SteamSetReturnMetadata(int64 queryHandle, bool returnMetadata)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetReturnMetadata(static_cast<UGCQueryHandle_t>(queryHandle), returnMetadata);
}

bool UAPIUGCClient::SteamSetReturnOnlyIDs(int64 queryHandle, bool returnOnlyIDs)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetReturnOnlyIDs(static_cast<UGCQueryHandle_t>(queryHandle), returnOnlyIDs);
}

bool UAPIUGCClient::SteamSetReturnPlaytimeStats(int64 queryHandle, int32 Days)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetReturnPlaytimeStats(static_cast<UGCQueryHandle_t>(queryHandle), static_cast<uint32>(Days));
}

bool UAPIUGCClient::SteamSetReturnTotalOnly(int64 queryHandle, bool returnTotalOnly)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetReturnTotalOnly(static_cast<UGCQueryHandle_t>(queryHandle), returnTotalOnly);
}

bool UAPIUGCClient::SteamSetSearchText(int64 queryHandle, FString searchText)
{
	STEAM_CLIENT(false);
	return SteamUGC()->SetSearchText(static_cast<UGCQueryHandle_t>(queryHandle), StringCast<ANSICHAR>(*searchText).Get());
}

bool UAPIUGCClient::SteamSetSearch(int64 queryHandle, FString searchText, bool allowCashedResponse, bool matchAnyTag, bool withAdditionalPreviews, bool withChildren, bool withKeyValueTags, bool withLongDescriptions, bool withMetadata)
{
	STEAM_CLIENT(false);
	bool response = SteamUGC()->SetSearchText(static_cast<UGCQueryHandle_t>(queryHandle), StringCast<ANSICHAR>(*searchText).Get());
	SteamUGC()->SetAllowCachedResponse(static_cast<UGCQueryHandle_t>(queryHandle), allowCashedResponse);
	SteamUGC()->SetMatchAnyTag(static_cast<UGCQueryHandle_t>(queryHandle), matchAnyTag);
	SteamUGC()->SetReturnAdditionalPreviews(static_cast<UGCQueryHandle_t>(queryHandle), withAdditionalPreviews);
	SteamUGC()->SetReturnChildren(static_cast<UGCQueryHandle_t>(queryHandle), withChildren);
	SteamUGC()->SetReturnKeyValueTags(static_cast<UGCQueryHandle_t>(queryHandle), withKeyValueTags);
	SteamUGC()->SetReturnLongDescription(static_cast<UGCQueryHandle_t>(queryHandle), withLongDescriptions);
	SteamUGC()->SetReturnMetadata(static_cast<UGCQueryHandle_t>(queryHandle), withMetadata);
	return response;
}

void UAPIUGCClient::SteamSetUserItemVote(int64 fileId, bool voteUp, FSetUserItemVoteResultCallback callback)
{
	STEAM_CLIENT(;);
	SetUserItemVoteResultLinker* linker = new SetUserItemVoteResultLinker(fileId, voteUp, callback);
}

int64 UAPIUGCClient::SteamStartItemUpdate(int32 appId, int64 fileId)
{
	STEAM_CLIENT(0);
	UGCUpdateHandle_t handle = SteamUGC()->StartItemUpdate(static_cast<AppId_t>(appId), static_cast<PublishedFileId_t>(fileId));
	return static_cast<int64>(handle);
}

void UAPIUGCClient::SteamStartPlaytimeTracking(TArray<int64> fileIDs, FStartPlaytimeTrackingResultCallback callback)
{
	STEAM_CLIENT(;);
	StartPlaytimeTrackingResultLinker* linker = new StartPlaytimeTrackingResultLinker(fileIDs, callback);
}

void UAPIUGCClient::SteamStopPlaytimeTracking(TArray<int64> fileIDs, FStopPlaytimeTrackingResultCallback callback)
{
	STEAM_CLIENT(;);
	StopPlaytimeTrackingResultLinker* linker = new StopPlaytimeTrackingResultLinker(fileIDs, callback);
}

void UAPIUGCClient::SteamStopPlaytimeTrackingForAllItems(FStopPlaytimeTrackingResultCallback callback)
{
	STEAM_CLIENT(;);
	StopAllPlaytimeTrackingResultLinker* linker = new StopAllPlaytimeTrackingResultLinker(callback);
}

void UAPIUGCClient::SteamSubmitItemUpdate(int64 updateHandle, FString changeNote, FSubmitItemUpdateResultCallback callback)
{
	STEAM_CLIENT(;);
	SubmitItemUpdateResultLinker* linker = new SubmitItemUpdateResultLinker(updateHandle, changeNote, callback);
}

void UAPIUGCClient::SteamSubscribeItem(int64 fileId, FRemoteStorageSubscribePublishedFileResultCallback callback)
{
	STEAM_CLIENT(;);
	RemoteStorageSubscribePublishedFileResultLinker* linker = new RemoteStorageSubscribePublishedFileResultLinker(fileId, callback);
}

void UAPIUGCClient::SteamSuspendDownloads(bool suspend)
{
	STEAM_CLIENT(;);
	SteamUGC()->SuspendDownloads(suspend);
}

void UAPIUGCClient::SteamUnsubscribeItem(int64 fileId, FRemoteStorageUnsubscribePublishedFileResultCallback callback)
{
	STEAM_CLIENT(;);
	RemoteStorageUnsubscribePublishedFileResultLinker* linker = new RemoteStorageUnsubscribePublishedFileResultLinker(fileId, callback);
}

bool UAPIUGCClient::SteamUpdateItemPreviewFile(int64 updateHandle, int32 index, FString previewFile)
{
	STEAM_CLIENT(false);
	return SteamUGC()->UpdateItemPreviewFile(static_cast<UGCUpdateHandle_t>(updateHandle), static_cast<uint32>(index), StringCast<ANSICHAR>(*previewFile).Get());
}

bool UAPIUGCClient::SteamUpdateItemPreviewVideo(int64 updateHandle, int32 index, FString videoId)
{
	STEAM_CLIENT(false);
	return SteamUGC()->UpdateItemPreviewVideo(static_cast<UGCUpdateHandle_t>(updateHandle), static_cast<uint32>(index), StringCast<ANSICHAR>(*videoId).Get());
}

bool UAPIUGCClient::SteamShowWorkshopEULA()
{
	STEAM_CLIENT(false);
	return SteamUGC()->ShowWorkshopEULA();
}

void UAPIUGCClient::SteamGetWorkshopEULAStatus(FWorkshopEULAStatusCallback callback)
{
	STEAM_CLIENT(;);
	WorkshopEULAStatusLinker* linker = new WorkshopEULAStatusLinker(callback);
}
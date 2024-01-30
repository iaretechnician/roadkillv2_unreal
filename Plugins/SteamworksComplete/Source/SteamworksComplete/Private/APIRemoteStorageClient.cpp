/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "APIRemoteStorageClient.h"

bool UAPIRemoteStorageClient::SteamBeginFileWriteBatch()
{
	STEAM_CLIENT(false);
	return SteamRemoteStorage()->BeginFileWriteBatch();
}

bool UAPIRemoteStorageClient::SteamEndFileWriteBatch()
{
	STEAM_CLIENT(false);
	return SteamRemoteStorage()->EndFileWriteBatch();;
}

bool UAPIRemoteStorageClient::SteamFileDelete(FString fileName)
{
	STEAM_CLIENT(false);
	return SteamRemoteStorage()->FileDelete(StringCast<ANSICHAR>(*fileName).Get());
}

bool UAPIRemoteStorageClient::SteamFileExists(FString fileName)
{
	STEAM_CLIENT(false);
	return SteamRemoteStorage()->FileExists(StringCast<ANSICHAR>(*fileName).Get());
}

bool UAPIRemoteStorageClient::SteamFileForget(FString fileName)
{
	STEAM_CLIENT(false);
	return SteamRemoteStorage()->FileForget(StringCast<ANSICHAR>(*fileName).Get());
}

TArray<uint8> UAPIRemoteStorageClient::SteamFileRead(FString fileName)
{
	STEAM_CLIENT(TArray<uint8>());

	if (SteamRemoteStorage()->FileExists(StringCast<ANSICHAR>(*fileName).Get()))
	{
		TArray<uint8> InSaveData;

		int32 fileSize = SteamRemoteStorage()->GetFileSize(StringCast<ANSICHAR>(*fileName).Get());
		InSaveData.SetNumUninitialized(fileSize);
		SteamRemoteStorage()->FileRead(StringCast<ANSICHAR>(*fileName).Get(), InSaveData.GetData(), fileSize);
		
		return InSaveData;
	}
	else
		return TArray<uint8>();
}

void UAPIRemoteStorageClient::SteamFileReadAsync(FString fileName, FSteamClientFileReadAsyncCallback callback)
{
	STEAM_CLIENT(;);
	RemoteStorageClientFileReadAsyncCompleteLinker* linker = new RemoteStorageClientFileReadAsyncCompleteLinker(fileName, callback);
}

void UAPIRemoteStorageClient::SteamFileShare(FString fileName, FSteamClientFileShareCallback callback)
{
	STEAM_CLIENT(;);
	FSteamClientFileShareLinker* linker = new FSteamClientFileShareLinker(fileName, callback);
}

bool UAPIRemoteStorageClient::SteamFileWrite(FString fileName, TArray<uint8> data)
{
	STEAM_CLIENT(false);
	return SteamRemoteStorage()->FileWrite(StringCast<ANSICHAR>(*fileName).Get(), data.GetData(), data.Num());
}

void UAPIRemoteStorageClient::SteamFileWriteAsync(FString fileName, TArray<uint8> data, FSteamClientFileWriteAsyncCallback callback)
{
	STEAM_CLIENT(;);
	RemoteStorageClientFileWriteAsyncCompleteLinker* linker = new RemoteStorageClientFileWriteAsyncCompleteLinker(fileName, data, callback);
}

bool UAPIRemoteStorageClient::SteamGetFileCount()
{
	STEAM_CLIENT(false);
	return false;
}

FRemoteStorageFileInfo UAPIRemoteStorageClient::SteamGetFileNameAndSize(int32 index)
{
	FRemoteStorageFileInfo result = FRemoteStorageFileInfo();
	STEAM_CLIENT(result);
	result.fileName = FString(SteamRemoteStorage()->GetFileNameAndSize(index, &(result.fileSize)));
	return result;
}

int32 UAPIRemoteStorageClient::SteamGetFileSize(FString fileName)
{
	STEAM_CLIENT(0);
	return SteamRemoteStorage()->GetFileSize(StringCast<ANSICHAR>(*fileName).Get());
}

FDateTime UAPIRemoteStorageClient::SteamGetFileTimestamp(FString fileName)
{
	STEAM_CLIENT(FDateTime());
	FDateTime result = FDateTime::FromUnixTimestamp(static_cast<int64>(SteamRemoteStorage()->GetFileTimestamp(StringCast<ANSICHAR>(*fileName).Get())));

	return result;
}

FRemoteStorageQuota UAPIRemoteStorageClient::SteamGetQuota()
{
	FRemoteStorageQuota result = FRemoteStorageQuota();
	STEAM_CLIENT(result);
	uint64 total = 0;
	uint64 available = 0;
	SteamRemoteStorage()->GetQuota(&total, &available);
	result.TotalCapacity = static_cast<int64>(total);
	result.Available = static_cast<int64>(available);
	return result;
}

UERemoteStoragePlatform UAPIRemoteStorageClient::SteamGetSyncPlatforms(FString fileName)
{
	STEAM_CLIENT(UERemoteStoragePlatform::EPC_None);
	return static_cast<UERemoteStoragePlatform>(SteamRemoteStorage()->GetSyncPlatforms(StringCast<ANSICHAR>(*fileName).Get()));
}

FRemoteStorageUGCDetails UAPIRemoteStorageClient::SteamGetUGCDetails(int64 ugcHandle)
{
	FRemoteStorageUGCDetails result = FRemoteStorageUGCDetails();
	AppId_t appId = AppId_t();
	uint32 length = 128;
	char* name = new char[length];
	int32 fileLength = 0;
	CSteamID owner = CSteamID();

	if (SteamRemoteStorage()->GetUGCDetails(static_cast<UGCHandle_t>(ugcHandle), &appId, &name, &fileLength, &owner))
	{
		// Copy the file details
		result.appId = appId;
		result.fileName = FString(name);
		result.fileSize = fileLength;
		result.ownerId = STEAMID_TO_INT64(owner);
	}

	delete[] name;

	return result;
}

FRemoteStorageUGCDownloadProgress UAPIRemoteStorageClient::SteamUGCDownloadProgress(int64 ugcHandle)
{
	FRemoteStorageUGCDownloadProgress result = FRemoteStorageUGCDownloadProgress();
	STEAM_CLIENT(result);
	int32 downloaded = 0;
	int32 expected = 0;
	if (SteamRemoteStorage()->GetUGCDownloadProgress(static_cast<UGCHandle_t>(ugcHandle), &downloaded, &expected))
	{
		result.Downloaded = downloaded;
		result.Expected = expected;
	}

	return result;
}

bool UAPIRemoteStorageClient::SteamIsCloudEnabledForAccount()
{
	STEAM_CLIENT(false);
	return SteamRemoteStorage()->IsCloudEnabledForAccount();
}

bool UAPIRemoteStorageClient::SteamIsCloudEnabledForApp()
{
	STEAM_CLIENT(false);
	return SteamRemoteStorage()->IsCloudEnabledForApp();
}

void UAPIRemoteStorageClient::SteamSetCloudEnabledForApp(bool enabled)
{
	STEAM_CLIENT(;);
	SteamRemoteStorage()->SetCloudEnabledForApp(enabled);
}

bool UAPIRemoteStorageClient::SteamSetSyncPlatforms(FString fileName, UERemoteStoragePlatform platform)
{
	STEAM_CLIENT(false);

	//All is max value of int32, Unreal handles enums as uint8 so we need to convert Unreal all manually to Valve All
	//for all other values the static cast is valid
	ERemoteStoragePlatform input = ERemoteStoragePlatform::k_ERemoteStoragePlatformAll;
	if (platform != UERemoteStoragePlatform::EPC_All)
		input = static_cast<ERemoteStoragePlatform>(platform);

	return SteamRemoteStorage()->SetSyncPlatforms(StringCast<ANSICHAR>(*fileName).Get(), input);
}
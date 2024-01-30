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
#include "APIRemoteStorageClient.generated.h"

UCLASS()
class STEAMWORKSCOMPLETE_API UAPIRemoteStorageClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamBeginFileWriteBatch();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamEndFileWriteBatch();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamFileDelete(FString fileName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamFileExists(FString fileName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamFileForget(FString fileName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<uint8> SteamFileRead(FString fileName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamFileReadAsync(FString fileName, FSteamClientFileReadAsyncCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamFileShare(FString fileName, FSteamClientFileShareCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamFileWrite(FString fileName, TArray<uint8> data);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamFileWriteAsync(FString fileName, TArray<uint8> data, FSteamClientFileWriteAsyncCallback callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamGetFileCount();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FRemoteStorageFileInfo SteamGetFileNameAndSize(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetFileSize(FString fileName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FDateTime SteamGetFileTimestamp(FString fileName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FRemoteStorageQuota SteamGetQuota();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UERemoteStoragePlatform SteamGetSyncPlatforms(FString fileName);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FRemoteStorageUGCDetails SteamGetUGCDetails(int64 ugcHandle);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FRemoteStorageUGCDownloadProgress SteamUGCDownloadProgress(int64 ugcHandle);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsCloudEnabledForAccount();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsCloudEnabledForApp();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamSetCloudEnabledForApp(bool enabled);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetSyncPlatforms(FString fileName, UERemoteStoragePlatform platform);
};

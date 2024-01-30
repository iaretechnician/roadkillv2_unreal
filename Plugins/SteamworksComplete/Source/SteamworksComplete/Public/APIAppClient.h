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
#include "APIAppClient.generated.h"

UCLASS()
class STEAMWORKSCOMPLETE_API UAPIAppClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamClientShouldRestart(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamClientInitialize();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamClientRunCallbacks();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FDLCData SteamGetDLCDataByIndex(int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsAppInstalled(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsCybercafe();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsDlcInstalled(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsLowViolence();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsSubscribed();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsSubscribedApp(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsSubscribedFromFamilySharing();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsSubscribedFromFreeWeekend();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FTimeTrial SteamIsTimedTrial();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsVACBanned();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetAppBuildId();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamnGetAppInstallDir(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetAppOwner();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<FString> SteamGetAvailableGameLanguages();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetCurrentBetaName();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetCurrentGameLanguage();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetDLCCount();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static float SteamGetDlcDownloadProgress(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FDateTime SteamGetEarliestPurchase(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static TArray<int32> SteamGetInstalledDepots(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetLaunchCommandLine();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetLaunchQueryParam(FString key);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamInstallDLC(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamMarkContentCorrupt(bool missingFilesOnly);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamUninstallDLC(int32 appId);
};

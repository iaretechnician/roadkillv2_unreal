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
#include "APIApp.generated.h"

UCLASS()
class STEAMWORKSCOMPLETE_API UAPIApp : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UAPIApp();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool IsSteamInitialized();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetAppId();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamShutdown();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetAppIdFromGameId(int64 gameId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetGameIdFromAppId(int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetSteamIdFromAccountId(int32 appId, UEAccountType type);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetAccountIdFromSteamId(int64 steamId);
};

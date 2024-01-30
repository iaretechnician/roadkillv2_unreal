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
#include "APIUserClient.generated.h"

UCLASS()
class STEAMWORKSCOMPLETE_API UAPIUserClient : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamGetMySteamId();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetMyFriendId();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetMySteamLevel();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsBehindNAT();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsPhoneIdentifying();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsPhoneRequiringVerification();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsPhoneVerified();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamIsTwoFactorEnabled();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamLoggedOn();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamCancelAuthTicket(int32 authTicketHandle);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FDecompressVoiceResult SteamDecompressVoice(TArray<uint8> CompressedData, int32 DesiredSampleRate, USoundWaveProcedural* buffer);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamEndAuthSession(int64 steamId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FAuthTicketData SteamGetAuthSessionTicket(int64 forSteamId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetAuthTicketForWebApi(FString identity);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FVoiceAvailableResult SteamGetAvailableVoice();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamGetEncryptedAppTicket(TArray<uint8> data, FEncryptedAppTicket callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetMyHex();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int64 SteamFriendIdToSteamId(int32 friendId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamFriendIdToHex(int32 friendId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamIdToFriendId(int64 steamId);

	UFUNCTION(BlueprintCallable, Category = "Steam ID")
	static FString SteamIdToHex(int64 steamId);

	UFUNCTION(BlueprintCallable, Category = "Steam ID")
	static FHexParseResults SteamHexToSteamId(FString hexId, UEAccountType type);

	UFUNCTION(BlueprintCallable, Category = "Steam ID")
	static int SteamHexToFriendId(FString hexId);

	UFUNCTION(BlueprintCallable, Category = "Steam ID")
	static bool SteamIdIsValid(int64 steamId);

	UFUNCTION(BlueprintCallable, Category = "Steam ID")
	static bool SteamIdIsMe(int64 steamId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamAdvertiseGame(int64 id, FString ip, int32 port);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UEBeginAuthSessionResult SteamBeginAuthSession(TArray<uint8> ticket, int64 steamId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int SteamGetGameBadgeLevel(int32 series, bool foil);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamRequestStoreAuthURL(FString RedirectUrl, FStoreAuthURLCallback Callback);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FVoiceResult SteamGetVoice();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetVoiceOptimalSampleRate();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamStartVoiceRecording();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamStopVoiceRecording();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UEUserHasLicenseForAppResult SteamUserHasLicenseForApp(int64 userId, int32 appId);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static bool SteamSetRichPresence(FString key, FString value);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static void SteamClearRichPresence();

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetRichPresence(FString key);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetUserRichPresence(int64 id, FString key);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static FString SteamGetUserRichPresenceKeyByIndex(int64 id, int32 index);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static int32 SteamGetUserRichPresenceKeyCount(int64 id);

	UFUNCTION(BlueprintCallable, Category = "Steamworks Complete")
	static UEPersonaState SteamGetUserPersonaState(int64 id);
};
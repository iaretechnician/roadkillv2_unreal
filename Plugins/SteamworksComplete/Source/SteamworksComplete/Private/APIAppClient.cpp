/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "APIAppClient.h"

bool UAPIAppClient::SteamClientShouldRestart(int32 appId)
{
#if WITH_EDITOR
	FFileHelper::SaveStringToFile(FString::FromInt(appId), TEXT("steam_appid.txt"));
	return false;
#else
	return SteamAPI_RestartAppIfNecessary(static_cast<AppId_t>(appId));
#endif
}

bool UAPIAppClient::SteamClientInitialize()
{
	return SteamAPI_Init();
}

void UAPIAppClient::SteamClientRunCallbacks()
{
	SteamAPI_RunCallbacks();
}

FDLCData UAPIAppClient::SteamGetDLCDataByIndex(int32 index)
{
	STEAM_CLIENT(FDLCData());
	
	FDLCData result;
	
	AppId_t appId;
	bool available;
	char name[128];
	if (SteamApps()->BGetDLCDataByIndex(index, &appId, &available, name, 128))
	{
		
		result.AppId = static_cast<int32>(appId);
		result.Available = available;
		result.Name = FString(name);
	}

	return result;
}

bool UAPIAppClient::SteamIsAppInstalled(int32 appId)
{
	STEAM_CLIENT(false);
	AppId_t app = static_cast<AppId_t>(appId);
	return SteamApps()->BIsAppInstalled(app);
}

bool UAPIAppClient::SteamIsCybercafe()
{
	STEAM_CLIENT(false);
	return SteamApps()->BIsCybercafe();
}

bool UAPIAppClient::SteamIsDlcInstalled(int32 appId)
{
	STEAM_CLIENT(false);
	AppId_t app = static_cast<AppId_t>(appId);
	return SteamApps()->BIsDlcInstalled(app);
}

bool UAPIAppClient::SteamIsLowViolence()
{
	STEAM_CLIENT(false);
	return SteamApps()->BIsLowViolence();
}

bool UAPIAppClient::SteamIsSubscribed()
{
	STEAM_CLIENT(false);
	return SteamApps()->BIsSubscribed();
}

bool UAPIAppClient::SteamIsSubscribedApp(int32 appId)
{
	STEAM_CLIENT(false);
	AppId_t app = static_cast<AppId_t>(appId);
	return SteamApps()->BIsSubscribedApp(app);
}

bool UAPIAppClient::SteamIsSubscribedFromFamilySharing()
{
	STEAM_CLIENT(false);
	return SteamApps()->BIsSubscribedFromFamilySharing();
}

bool UAPIAppClient::SteamIsSubscribedFromFreeWeekend()
{
	STEAM_CLIENT(false);
	return SteamApps()->BIsSubscribedFromFreeWeekend();
}

FTimeTrial UAPIAppClient::SteamIsTimedTrial()
{
	STEAM_CLIENT(FTimeTrial());
	uint32 played;
	uint32 max;
	bool isTT = SteamApps()->BIsTimedTrial(&max, &played);
	FTimeTrial result;
	result.IsTrial = isTT;
	result.AllowedSeconds = max;
	result.PlayedSeconds = played;
	return result;
}

bool UAPIAppClient::SteamIsVACBanned()
{
	STEAM_CLIENT(false);
	return SteamApps()->BIsVACBanned();
}

int32 UAPIAppClient::SteamGetAppBuildId()
{
	STEAM_CLIENT(0);
	return SteamApps()->GetAppBuildId();
}

FString UAPIAppClient::SteamnGetAppInstallDir(int32 appId)
{
	STEAM_CLIENT(FString());
	AppId_t app = static_cast<AppId_t>(appId);
	char buffer[1024];
	int32 writen = SteamApps()->GetAppInstallDir(app, buffer, 1024);

	if (writen > 0)
		return FString(buffer, writen);
	else
		return FString();
}

int64 UAPIAppClient::SteamGetAppOwner()
{
	STEAM_CLIENT(0);
	return STEAMID_TO_INT64(SteamApps()->GetAppOwner());
}

TArray<FString> UAPIAppClient::SteamGetAvailableGameLanguages()
{
	STEAM_CLIENT(TArray<FString>());

	const char* result = SteamApps()->GetAvailableGameLanguages();
	FString raw(result);
	TArray<FString> languages;
	raw.ParseIntoArray(languages, TEXT(","));

	return languages;
}

FString UAPIAppClient::SteamGetCurrentBetaName()
{
	STEAM_CLIENT(FString());
	return FString();
}

FString UAPIAppClient::SteamGetCurrentGameLanguage()
{
	STEAM_CLIENT(FString());
	char buffer[128];
	SteamApps()->GetCurrentBetaName(buffer, 128);
	return FString(buffer);
}

int32 UAPIAppClient::SteamGetDLCCount()
{
	STEAM_CLIENT(0);
	return SteamApps()->GetDLCCount();
}

float UAPIAppClient::SteamGetDlcDownloadProgress(int32 appId)
{
	STEAM_CLIENT(0.0f);
	AppId_t app = static_cast<AppId_t>(appId);
	uint64 cur;
	uint64 max;
	SteamApps()->GetDlcDownloadProgress(app, &cur, &max);

	if (max > 0)
		return static_cast<float>(cur) / static_cast<float>(max);
	else
		return 0.0f;
}

FDateTime UAPIAppClient::SteamGetEarliestPurchase(int32 appId)
{
	STEAM_CLIENT(FDateTime());
	AppId_t app = static_cast<AppId_t>(appId);
	
	return FDateTime::FromUnixTimestamp(SteamApps()->GetEarliestPurchaseUnixTime(app));
}

TArray<int32> UAPIAppClient::SteamGetInstalledDepots(int32 appId)
{
	STEAM_CLIENT(TArray<int32>());
	AppId_t app = static_cast<AppId_t>(appId);
	DepotId_t depots[64];
	int32 count = SteamApps()->GetInstalledDepots(app, depots, 64);

	if (count <= 0)
		return TArray<int32>();
	else
	{
		TArray<int32> results;
		for (int i = 0; i < count; i++)
			results.Add(static_cast<int32>(depots[i]));

		return results;
	}
}

FString UAPIAppClient::SteamGetLaunchCommandLine()
{
	STEAM_CLIENT(FString());
	char commandLine[256];
	int32 count = SteamApps()->GetLaunchCommandLine(commandLine, 256);
	if (count > 0)
		return FString(commandLine);
	else
		return FString();
}

FString UAPIAppClient::SteamGetLaunchQueryParam(FString key)
{
	STEAM_CLIENT(FString());
	const char* value = SteamApps()->GetLaunchQueryParam(StringCast<ANSICHAR>(*key).Get());

	return FString(value);
}

void UAPIAppClient::SteamInstallDLC(int32 appId)
{
	STEAM_CLIENT(;);

	SteamApps()->InstallDLC(static_cast<AppId_t>(appId));
}

bool UAPIAppClient::SteamMarkContentCorrupt(bool missingFilesOnly)
{
	STEAM_CLIENT(false);
	return SteamApps()->MarkContentCorrupt(missingFilesOnly);
}

void UAPIAppClient::SteamUninstallDLC(int32 appId)
{
	STEAM_CLIENT(;);
	AppId_t app = static_cast<AppId_t>(appId);
	SteamApps()->UninstallDLC(app);
}
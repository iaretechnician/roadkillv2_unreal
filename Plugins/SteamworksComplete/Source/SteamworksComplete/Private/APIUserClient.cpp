/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "APIUserClient.h"

int64 UAPIUserClient::SteamGetMySteamId()
{
	STEAM_CLIENT(0);
	CSteamID id = SteamUser()->GetSteamID();
	return STEAMID_TO_INT64(id);
}

int32 UAPIUserClient::SteamGetMyFriendId()
{
	STEAM_CLIENT(0);

	CSteamID id = SteamUser()->GetSteamID();
	return static_cast<int32>(id.GetAccountID());
}

int32 UAPIUserClient::SteamGetMySteamLevel()
{
	STEAM_CLIENT(0);
	return SteamUser()->GetPlayerSteamLevel();
}

bool UAPIUserClient::SteamIsBehindNAT()
{
	STEAM_CLIENT(false);

	return SteamUser()->BIsBehindNAT();
}

bool UAPIUserClient::SteamIsPhoneIdentifying()
{
	STEAM_CLIENT(false);

	return SteamUser()->BIsPhoneIdentifying();
}

bool UAPIUserClient::SteamIsPhoneRequiringVerification()
{
	STEAM_CLIENT(false);

	return SteamUser()->BIsPhoneRequiringVerification();
}

bool UAPIUserClient::SteamIsPhoneVerified()
{
	STEAM_CLIENT(false);

	return SteamUser()->BIsPhoneVerified();
}

bool UAPIUserClient::SteamIsTwoFactorEnabled()
{
	STEAM_CLIENT(false);

	return SteamUser()->BIsTwoFactorEnabled();
}

bool UAPIUserClient::SteamLoggedOn()
{
	STEAM_CLIENT(false);

	return SteamUser()->BLoggedOn();
}

void UAPIUserClient::SteamCancelAuthTicket(int32 authTicketHandle)
{
	STEAM_CLIENT(;);
	SteamUser()->CancelAuthTicket(static_cast<HAuthTicket>(authTicketHandle));
}

FDecompressVoiceResult UAPIUserClient::SteamDecompressVoice(TArray<uint8> CompressedData, int32 DesiredSampleRate, USoundWaveProcedural* buffer)
{
	STEAM_CLIENT(FDecompressVoiceResult());

	FDecompressVoiceResult Result;

	// Convert the FString CompressedData to a byte array (uint8)
	TArray<uint8> CompressedVoiceData;
	int32 CompressedSize = CompressedData.Num() * sizeof(uint8);
	CompressedVoiceData.SetNumUninitialized(CompressedSize);
	FMemory::Memcpy(CompressedVoiceData.GetData(), CompressedData.GetData(), CompressedSize);

	// Calculate the destination buffer size based on the compression ratio
	int32 DestBufferSize = CompressedSize * 4;
	// Set up the destination buffer with the specified size
	Result.DecompressedVoiceData.SetNumUninitialized(DestBufferSize);

	// Call the actual DecompressVoice function with the appropriate arguments
	uint32 BytesWritten = 0;
	EVoiceResult result = SteamUser()->DecompressVoice(CompressedVoiceData.GetData(), CompressedSize, Result.DecompressedVoiceData.GetData(), DestBufferSize, &BytesWritten, DesiredSampleRate);

	if (BytesWritten > 0)
	{
		Result.DecompressedVoiceData.SetNum(BytesWritten);
		Result.Result = static_cast<UEVoiceResult>(result);

		// Create a UProceduralSoundWave instance
		if(buffer == nullptr)
			buffer = NewObject<USoundWaveProcedural>();

		// Set the sample rate, number of channels, duration, and allocate memory for audio data
		buffer->SetSampleRate(DesiredSampleRate);
		buffer->NumChannels = 1;
		buffer->SoundGroup = SOUNDGROUP_Voice;
		buffer->Duration = INDEFINITELY_LOOPING_DURATION;
		
		// Allocate memory for audio data and copy PCM audio data into the USoundWaveProcedural
		buffer->QueueAudio(Result.DecompressedVoiceData.GetData(), BytesWritten);

		// Assign the UProceduralSoundWave to the result
		Result.ProceduralSoundWave = buffer;

		Result.Success = true;
	}
	else if (result == EVoiceResult::k_EVoiceResultNoData)
	{
		Result.Result = static_cast<UEVoiceResult>(result);
		Result.Success = true;
	}
	else
	{
		Result.Result = static_cast<UEVoiceResult>(result);
		Result.Success = false;
	}

	return Result;
}

void UAPIUserClient::SteamEndAuthSession(int64 steamId)
{
	STEAM_CLIENT(;);
	SteamUser()->EndAuthSession(INT64_TO_STEAMID(steamId));
}

FAuthTicketData UAPIUserClient::SteamGetAuthSessionTicket(int64 forSteamId)
{
	STEAM_CLIENT(FAuthTicketData());

	TArray<uint8> buffer;
	buffer.SetNumUninitialized(1024);
	uint32 BytesWritten = 0;
	SteamNetworkingIdentity Id{};
	Id.m_eType = ESteamNetworkingIdentityType::k_ESteamNetworkingIdentityType_SteamID;
	Id.SetSteamID(INT64_TO_STEAMID(forSteamId));
	HAuthTicket handle = SteamUser()->GetAuthSessionTicket(buffer.GetData(), 1024, &BytesWritten, &Id);

	FAuthTicketData result;
	result.authTicketHandle = static_cast<int32>(handle);
	result.Ticket = buffer;

	return result;
}

int32 UAPIUserClient::SteamGetAuthTicketForWebApi(FString identity)
{
	STEAM_CLIENT(0);

	if (identity.IsEmpty())
	{
		HAuthTicket handle = SteamUser()->GetAuthTicketForWebApi(NULL);
		return static_cast<int32>(handle);
	}
	else
	{
		HAuthTicket handle = SteamUser()->GetAuthTicketForWebApi(StringCast<ANSICHAR>(*identity).Get());
		return static_cast<int32>(handle);
	}
}

FVoiceAvailableResult UAPIUserClient::SteamGetAvailableVoice()
{
	FVoiceAvailableResult result = FVoiceAvailableResult();
	STEAM_CLIENT(result);
	uint32 pData = 0;
	result.Result = static_cast<UEVoiceResult>(SteamUser()->GetAvailableVoice(&pData));
	result.VoiceData = static_cast<int32>(pData);
	return result;
}

void UAPIUserClient::SteamGetEncryptedAppTicket(TArray<uint8> data, FEncryptedAppTicket callback)
{
	STEAM_CLIENT(;);
	APIUserClientEncryptedAppTicketLinker* linker = new APIUserClientEncryptedAppTicketLinker(data, callback);
}

FString UAPIUserClient::SteamGetMyHex()
{
	STEAM_CLIENT(FString());

	CSteamID id = SteamUser()->GetSteamID();
	return FString::Printf(TEXT("%X"), id.GetAccountID());
}

int64 UAPIUserClient::SteamFriendIdToSteamId(int32 friendId)
{
	CSteamID steamId(friendId, EUniverse::k_EUniversePublic, EAccountType::k_EAccountTypeIndividual);
	return static_cast<int64>(steamId.ConvertToUint64());
}

FString UAPIUserClient::SteamFriendIdToHex(int32 friendId)
{
	return FString::Printf(TEXT("%X"), friendId);
}

int UAPIUserClient::SteamIdToFriendId(int64 steamId)
{
	CSteamID id(static_cast<uint64>(steamId));
	return static_cast<int>(id.GetAccountID());
}

FString UAPIUserClient::SteamIdToHex(int64 steamId)
{
	CSteamID id = INT64_TO_STEAMID(steamId);

	uint32 steamAccount = id.GetAccountID();
	FString hexId = FString::Printf(TEXT("%X"), id.GetAccountID());
	int parsed = FParse::HexNumber(*hexId);
	
	UE_LOG(LogTemp, Log, TEXT("Source : %u | IN : %u | OUT : %s | Proof : %d | Type : %d | Instance : %u"), steamId, steamAccount, *hexId, parsed, id.GetEAccountType(), id.GetUnAccountInstance());

	return FString::Printf(TEXT("%X"), id.GetAccountID());
}

FHexParseResults UAPIUserClient::SteamHexToSteamId(FString hexId, UEAccountType type)
{
	FHexParseResults parseResult = FHexParseResults();	
	int Result = FParse::HexNumber(*hexId);

	// Handle overflow or conversion errors
	if (Result == 0 && hexId != TEXT("0"))
	{
		parseResult.success = false;
		UE_LOG(LogTemp, Warning, TEXT("Hexadecimal string conversion overflow or error."));
	}
	else
	{
		parseResult.success = true;
	}

	if (type == UEAccountType::EPC_Chat)
	{
		CSteamID steamId = CSteamID(Result, 393216, EUniverse::k_EUniversePublic, static_cast<EAccountType>(type));
		parseResult.steamId = STEAMID_TO_INT64(steamId);
		UE_LOG(LogTemp, Log, TEXT("Lobby ID Parse : %s | Result : %d | ID : %lld"), *hexId, Result, parseResult.steamId);
	}
	else
	{
		CSteamID steamId = CSteamID(Result, EUniverse::k_EUniversePublic, static_cast<EAccountType>(type));
		parseResult.steamId = STEAMID_TO_INT64(steamId);
		UE_LOG(LogTemp, Log, TEXT("ID Parse : %s | Result : %d | ID : %lld"), *hexId, Result, parseResult.steamId);
	}

	return parseResult;
}

int32 UAPIUserClient::SteamHexToFriendId(FString hexId)
{
	FHexParseResults parseResult = SteamHexToSteamId(hexId, UEAccountType::EPC_Individual);

	return parseResult.steamId;
}

bool UAPIUserClient::SteamIdIsValid(int64 steamId)
{
	CSteamID id = INT64_TO_STEAMID(steamId);
	id.IsValid();
	return id.IsValid();
}

bool UAPIUserClient::SteamIdIsMe(int64 steamId)
{
	CSteamID id = INT64_TO_STEAMID(steamId);
	CSteamID me = SteamUser()->GetSteamID();
	return id == me;
}

void UAPIUserClient::SteamAdvertiseGame(int64 id, FString ip, int32 port)
{
	STEAM_CLIENT(;);

	// Split the IP address into octets
	TArray<FString> OctetStrings;
	ip.ParseIntoArray(OctetStrings, TEXT("."), true);

	uint32 address = 0;

	if (OctetStrings.Num() == 4)
	{
		// Convert octets to integers
		uint8 Octets[4];
		for (int32 i = 0; i < 4; ++i)
		{
			Octets[i] = FCString::Atoi(*OctetStrings[i]);
		}

		// Calculate the uint32 representation of the IP address
		address = (Octets[0] << 24) | (Octets[1] << 16) | (Octets[2] << 8) | Octets[3];
	}

	CSteamID steamId(static_cast<uint64>(id));

	SteamUser()->AdvertiseGame(steamId, address, static_cast<uint16>(port > 65535 || port < 0 ? 0 : port));
}

UEBeginAuthSessionResult UAPIUserClient::SteamBeginAuthSession(TArray<uint8> ticket, int64 steamId)
{
	STEAM_CLIENT(UEBeginAuthSessionResult::EPC_InvalidTicket);

	// Convert the FString AuthTicket to a byte array (uint8)
	TArray<uint8> TicketData;
	int32 TicketLength = ticket.Num() * sizeof(uint8); // Get the length dynamically

	TicketData.SetNumUninitialized(TicketLength);
	FMemory::Memcpy(TicketData.GetData(), ticket.GetData(), TicketLength);

	// Call the actual BeginAuthSession function with the appropriate arguments
	EBeginAuthSessionResult result = SteamUser()->BeginAuthSession(TicketData.GetData(), TicketLength, INT64_TO_STEAMID(steamId));

	return static_cast<UEBeginAuthSessionResult>(result);
}

int32 UAPIUserClient::SteamGetGameBadgeLevel(int32 series, bool foil)
{
	STEAM_CLIENT(0);

	return SteamUser()->GetGameBadgeLevel(series, foil);
}

void UAPIUserClient::SteamRequestStoreAuthURL(FString RedirectUrl, FStoreAuthURLCallback Callback)
{
	STEAM_CLIENT(;);
	APIUserClientCallbackLinker* Linker = new APIUserClientCallbackLinker(RedirectUrl, Callback);
}

FVoiceResult UAPIUserClient::SteamGetVoice()
{
	FVoiceResult defaultVal;
	defaultVal.Result = UEVoiceResult::EPC_NoData;
	STEAM_CLIENT(defaultVal);

	uint32 availableData;
	EVoiceResult result = SteamUser()->GetAvailableVoice(&availableData);

	if (availableData == 0)
		return defaultVal;
	
	defaultVal.VoiceData.SetNumUninitialized(availableData);
	result = SteamUser()->GetVoice(true, defaultVal.VoiceData.GetData(), availableData, &availableData);
	defaultVal.Result = static_cast<UEVoiceResult>(result);

	return defaultVal;
}

int32 UAPIUserClient::SteamGetVoiceOptimalSampleRate()
{
	STEAM_CLIENT(0);
	uint32 result = SteamUser()->GetVoiceOptimalSampleRate();
	return static_cast<int32>(result);
}

void UAPIUserClient::SteamStartVoiceRecording()
{
	STEAM_CLIENT(;);
	SteamUser()->StartVoiceRecording();
}

void UAPIUserClient::SteamStopVoiceRecording()
{
	STEAM_CLIENT(;);
	SteamUser()->StopVoiceRecording();
}

UEUserHasLicenseForAppResult UAPIUserClient::SteamUserHasLicenseForApp(int64 userId, int32 appId)
{
	STEAM_CLIENT(UEUserHasLicenseForAppResult::EPC_NoAuth);
	CSteamID steamId = INT64_TO_STEAMID(userId);
	AppId_t app = static_cast<AppId_t>(appId);
	EUserHasLicenseForAppResult result = SteamUser()->UserHasLicenseForApp(steamId, app);
	return static_cast<UEUserHasLicenseForAppResult>(result);
}

bool UAPIUserClient::SteamSetRichPresence(FString key, FString value)
{
	STEAM_CLIENT(false);
	return SteamFriends()->SetRichPresence(StringCast<ANSICHAR>(*key).Get(), StringCast<ANSICHAR>(*value).Get());
}

void UAPIUserClient::SteamClearRichPresence()
{
	STEAM_CLIENT(;);
	SteamFriends()->ClearRichPresence();
}

FString UAPIUserClient::SteamGetRichPresence(FString key)
{
	STEAM_CLIENT(FString());
	const char* result = SteamFriends()->GetFriendRichPresence(SteamUser()->GetSteamID(), StringCast<ANSICHAR>(*key).Get());
	return FString(StringCast<TCHAR>(result));
}

FString UAPIUserClient::SteamGetUserRichPresence(int64 id, FString key)
{
	CSteamID steamId = INT64_TO_STEAMID(id);
	STEAM_CLIENT(FString());
	
	if (!steamId.IsValid())
		return FString();

	const char* result = SteamFriends()->GetFriendRichPresence(steamId, StringCast<ANSICHAR>(*key).Get());
	return FString(result);
}

FString UAPIUserClient::SteamGetUserRichPresenceKeyByIndex(int64 id, int32 index)
{
	STEAM_CLIENT(FString());
	CSteamID steamId = INT64_TO_STEAMID(id);
	
	if (!steamId.IsValid())
		return FString();

	const char* result = SteamFriends()->GetFriendRichPresenceKeyByIndex(steamId, index);
	return FString(result);
}

int32 UAPIUserClient::SteamGetUserRichPresenceKeyCount(int64 id)
{
	STEAM_CLIENT(0);
	CSteamID steamId = INT64_TO_STEAMID(id);

	return SteamFriends()->GetFriendRichPresenceKeyCount(steamId);
}

UEPersonaState UAPIUserClient::SteamGetUserPersonaState(int64 id)
{
	STEAM_CLIENT(UEPersonaState::EPC_Offline);
	EPersonaState state = SteamFriends()->GetFriendPersonaState(INT64_TO_STEAMID(id));
	return static_cast<UEPersonaState>(state);
}

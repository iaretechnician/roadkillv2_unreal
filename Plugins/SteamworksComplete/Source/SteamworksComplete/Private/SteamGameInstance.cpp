/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "SteamGameInstance.h"

// Log Category for The API Debugger
DEFINE_LOG_CATEGORY_STATIC(LogSteamworksCompleteSocketsAPI, Log, All);

/** Message logging hook for debugging Steam Sockets. */
void SteamworksCompleteSocketsDebugLogger(ESteamNetworkingSocketsDebugOutputType nType, const char* pszMsg)
{
#if !NO_LOGGING
    FString OutputType;
    ELogVerbosity::Type Verboseness = ELogVerbosity::Verbose;
    switch (nType)
    {
    default:
    case k_ESteamNetworkingSocketsDebugOutputType_None:
        OutputType = TEXT("None");
        break;
    case k_ESteamNetworkingSocketsDebugOutputType_Bug:
        OutputType = TEXT("Bug");
        Verboseness = ELogVerbosity::Error;
        break;
    case k_ESteamNetworkingSocketsDebugOutputType_Error:
        OutputType = TEXT("Error");
        Verboseness = ELogVerbosity::Error;
        break;
    case k_ESteamNetworkingSocketsDebugOutputType_Important:
        OutputType = TEXT("Important");
        Verboseness = ELogVerbosity::Warning;
        break;
    case k_ESteamNetworkingSocketsDebugOutputType_Warning:
        OutputType = TEXT("Warning");
        Verboseness = ELogVerbosity::Warning;
        break;
    case k_ESteamNetworkingSocketsDebugOutputType_Everything:
    case k_ESteamNetworkingSocketsDebugOutputType_Verbose:
    case k_ESteamNetworkingSocketsDebugOutputType_Msg:
        OutputType = TEXT("Log");
        break;
    case k_ESteamNetworkingSocketsDebugOutputType_Debug:
        OutputType = TEXT("Debug");
        break;
    }

    // Runtime detection of logging level and verbosity.
    GLog->Log(LogSteamworksCompleteSocketsAPI.GetCategoryName(), Verboseness, FString::Printf(TEXT("SteamworksCompleteSockets API: %s %s"), *OutputType, ANSI_TO_TCHAR(pszMsg)));
#endif
}

USteamGameInstance* USteamGameInstance::pInstance;

void USteamGameInstance::Shutdown()
{   
    pInstance = nullptr;
    GetWorld()->GetTimerManager().ClearTimer(SteamCallbackTimerHandle);

    if(IsDedicatedServerInstance())
        SteamGameServer_Shutdown();
    else
        SteamAPI_Shutdown();

    Super::Shutdown();
}

void USteamGameInstance::Init()
{
    pInstance = this;
    Super::Init();
}

USteamGameInstance* USteamGameInstance::GetSteamGameInstance()
{
    //UWorld* World = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport)->World();
    //return Cast<USteamGameInstance>(World->GetGameInstance());
    return pInstance;
}

FSteamworksInitializationResponse USteamGameInstance::InitializeSteamAPI()
{
    FSteamworksInitializationResponse result = FSteamworksInitializationResponse();

    //Create the steam_appid.txt on init if this is running in editor
    if (IsDedicatedServerInstance())   
    { 
        uint32 uIp = HeathenTools::IPAddress(ip);
        uint16 gPort = static_cast<uint16>(gamePort);
        uint16 qPort = static_cast<uint16>(queryPort);
        result.success = SteamGameServer_Init(uIp, gPort, qPort, static_cast<EServerMode>(mode), StringCast<ANSICHAR>(*version).Get());
        result.shouldRestart = false;
    }
    else
    {
        bool requiresRestart = false;

#if WITH_EDITOR
        FFileHelper::SaveStringToFile(FString::FromInt(appId), TEXT("steam_appid.txt"));
#else
        requiresRestart = SteamAPI_RestartAppIfNecessary(static_cast<AppId_t>(appId));
#endif

        if(!requiresRestart)
        {
            result.shouldRestart = false;
            result.success = SteamAPI_Init();
        }
        else
        {
            result.shouldRestart = true;
            result.success = false;
        }
    }

    if (result.success)
    {
        FTimerDelegate TimerDelegate;
        callbackFrequency = FMath::Clamp(callbackFrequency, 0.01f, 0.1f);
        TimerDelegate.BindLambda([this]() {
            if (this->IsDedicatedServerInstance())
            {
                SteamGameServer_RunCallbacks();
            }
            else
            {
                SteamAPI_RunCallbacks();
            }
            });
        GetWorld()->GetTimerManager().SetTimer(SteamCallbackTimerHandle, TimerDelegate, callbackFrequency, true);
    }

    return result;
}

void USteamGameInstance::InitializeRelayNetworkAccess()
{
    SteamNetworkingUtils()->InitRelayNetworkAccess();
}

void USteamGameInstance::InitializeNetworkingDebugOutput()
{
    // Set up the API Logging
    ESteamNetworkingSocketsDebugOutputType DebugLevel = k_ESteamNetworkingSocketsDebugOutputType_Msg;

    // Set this a bit higher for shipping builds because SteamworksCompleteSockets can spew a lot of information
#if UE_BUILD_SHIPPING
    DebugLevel = k_ESteamNetworkingSocketsDebugOutputType_Important;
#endif

    SteamNetworkingUtils()->SetDebugOutputFunction(DebugLevel, SteamworksCompleteSocketsDebugLogger);
}

void USteamGameInstance::LoadIcon(int handle, FString apiName, FIconLoadCallback callback)
{
    if (handle > 0)
    {
        UTexture2D* IconTexture = UTexture2D::CreateTransient(STEAMWORKS_ICON_SIZE, STEAMWORKS_ICON_SIZE, PF_R8G8B8A8);
        AchievementIcons.Add(apiName, IconTexture);

        TArray<uint8> Buffer;
        Buffer.SetNumZeroed(STEAMWORKS_ICON_BYTE_SIZE);

        if (SteamUtils()->GetImageRGBA(handle, Buffer.GetData(), Buffer.Num()))
        {
            void* MipData = IconTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
            FMemory::Memcpy(MipData, Buffer.GetData(), Buffer.Num());
            IconTexture->GetPlatformData()->Mips[0].BulkData.Unlock();

            IconTexture->GetPlatformData()->SetNumSlices(1);
            IconTexture->NeverStream = true;

            IconTexture->UpdateResource();
        }
        else
        {
            ensure(false && "this shouldn't happen");
        }

        callback.Execute(IconTexture);
    }
}

void USteamGameInstance::GetAvatarBySteamId(CSteamID SteamId, FAvatarLoadCallback Callback)
{
    if (!SteamId.IsValid())
    {
        return;
    }

    uint64 uint64SteamId = SteamId.ConvertToUint64();
    if (Avatars.Contains(uint64SteamId))
    {
        UTexture2D* avatarTexture = Avatars.FindRef(SteamId.ConvertToUint64());

        if (Callback.IsBound())
            Callback.Execute(Avatars.FindRef(SteamId.ConvertToUint64()));
    }
    else
    {
        if (AvatarRequests.Contains(uint64SteamId))
            AvatarRequests.Remove(uint64SteamId);

        AvatarRequests.Add(uint64SteamId, Callback);

        if (!SteamFriends()->RequestUserInformation(SteamId, false))
            LoadSteamAvatar(SteamId);
    }
}

void USteamGameInstance::LoadSteamAvatar(CSteamID SteamId)
{
    if (SteamFriends() != nullptr && SteamId.IsValid())
    {
        int Handle = SteamFriends()->GetLargeFriendAvatar(SteamId);

        if (Handle > 0)
        {
            UTexture2D* AvatarTexture = UTexture2D::CreateTransient(STEAMWORKS_AVATAR_SIZE, STEAMWORKS_AVATAR_SIZE, PF_R8G8B8A8);
            Avatars.Add(SteamId.ConvertToUint64(), AvatarTexture);
            CopySteamAvatar(Handle, AvatarTexture);

            uint64 uint64SteamId = SteamId.ConvertToUint64();
            if (AvatarRequests.Contains(uint64SteamId))
            {
                FAvatarLoadCallback Callback = AvatarRequests.FindRef(uint64SteamId);
                if (Callback.IsBound())
                    Callback.Execute(AvatarTexture);

                AvatarRequests.Remove(uint64SteamId);
            }
        }
    }
}

void USteamGameInstance::CopySteamAvatar(int Handle, UTexture2D* AvatarTexture)
{
    check(Handle > 0);
    check(AvatarTexture != nullptr);

    TArray<uint8> Buffer;
    Buffer.SetNumZeroed(STEAMWORKS_AVATAR_BYTE_SIZE);

    //copy it directly within this call
    if (SteamUtils()->GetImageRGBA(Handle, Buffer.GetData(), Buffer.Num()))
    {
        void* MipData = AvatarTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
        FMemory::Memcpy(MipData, Buffer.GetData(), Buffer.Num());
        AvatarTexture->GetPlatformData()->Mips[0].BulkData.Unlock();

        AvatarTexture->GetPlatformData()->SetNumSlices(1);
        AvatarTexture->NeverStream = true;

        AvatarTexture->UpdateResource();
    }
    else
    {
        ensure(false && "this shouldn't happen");
    }
}

void USteamGameInstance::EventUserStatsReceived(UserStatsReceived_t* arg0)
{
    if (SteamUserStatsReceived.IsBound())
        SteamUserStatsReceived.Broadcast(static_cast<int64>(arg0->m_nGameID), static_cast<UEResult>(arg0->m_eResult), STEAMID_TO_INT64(arg0->m_steamIDUser));
}

void USteamGameInstance::EventUserStatsUnloaded(UserStatsUnloaded_t* arg0)
{
    if (SteamUserStatsUnloaded.IsBound())
        SteamUserStatsUnloaded.Broadcast(STEAMID_TO_INT64(arg0->m_steamIDUser));
}

void USteamGameInstance::EventUserStatsStored(UserStatsStored_t* arg0)
{
    if (SteamUserStatsStored.IsBound())
        SteamUserStatsStored.Broadcast(static_cast<int64>(arg0->m_nGameID));
}

void USteamGameInstance::EventUserAchievementStored(UserAchievementStored_t* arg0)
{
    if (SteamUserAchievementStored.IsBound())
        SteamUserAchievementStored.Broadcast(static_cast<int64>(arg0->m_nGameID), FString(arg0->m_rgchAchievementName), static_cast<int32>(arg0->m_nCurProgress), static_cast<int32>(arg0->m_nMaxProgress));
}

void USteamGameInstance::EventUserAchievementIconFetched(UserAchievementIconFetched_t* arg0)
{
    if (arg0->m_nIconHandle > 0)
    {
        if (AchievementIconRequests.GetAllocatedSize() == 0)
            AchievementIconRequests = TMap<FString, FIconLoadCallback>();

        FString key(arg0->m_rgchAchievementName);

        if (AchievementIconRequests.Contains(key))
        {
            AchievementIconRequests.Remove(key);
            LoadIcon(arg0->m_nIconHandle, key, AchievementIconRequests.FindRef(key));
        }
    }
}

void USteamGameInstance::EventDlcInstalled(DlcInstalled_t* pParam)
{
    if (SteamDlcInstalled.IsBound())
        SteamDlcInstalled.Broadcast(static_cast<int32>(pParam->m_nAppID));
}

void USteamGameInstance::EventSteamServerConnectFailure(SteamServerConnectFailure_t* arg0)
{
    if (SteamServerConnectFailure.IsBound())
        SteamServerConnectFailure.Broadcast(static_cast<UEResult>(arg0->m_eResult), arg0->m_bStillRetrying);
}

void USteamGameInstance::EventSteamServersConnected(SteamServersConnected_t* arg0)
{
    if (SteamServersConnected.IsBound())
        SteamServersConnected.Broadcast();
}

void USteamGameInstance::EventSteamServersDisconnected(SteamServersDisconnected_t* arg0)
{
    if (SteamServersDisconnected.IsBound())
        SteamServersDisconnected.Broadcast(static_cast<UEResult>(arg0->m_eResult));
}

void USteamGameInstance::EventSteamGameServerConnectFailure(SteamServerConnectFailure_t* arg0)
{
    if (SteamGameServerConnectionChanged.IsBound())
    {
        SteamGameServerConnectionChanged.Execute(false);
    }

    if (SteamServerConnectFailure.IsBound())
        SteamServerConnectFailure.Broadcast(static_cast<UEResult>(arg0->m_eResult), arg0->m_bStillRetrying);
}

void USteamGameInstance::EventSteamGameServersConnected(SteamServersConnected_t* arg0)
{
    if (SteamGameServerConnectionChanged.IsBound())
    {
        SteamGameServerConnectionChanged.Execute(true);
    }

    if (SteamServersConnected.IsBound())
        SteamServersConnected.Broadcast();
}

void USteamGameInstance::EventSteamGameServersDisconnected(SteamServersDisconnected_t* arg0)
{
    if (SteamGameServerConnectionChanged.IsBound())
    {
        SteamGameServerConnectionChanged.Execute(false);
    }

    if (SteamServersDisconnected.IsBound())
        SteamServersDisconnected.Broadcast(static_cast<UEResult>(arg0->m_eResult));
}

void USteamGameInstance::EventAvatarImageLoaded(AvatarImageLoaded_t* arg0)
{
    uint64 uint64SteamId = arg0->m_steamID.ConvertToUint64();

    if (Avatars.Contains(uint64SteamId))
    {
        UTexture2D* AvatarTexture = Avatars.FindRef(uint64SteamId);
        int Handle = SteamFriends()->GetLargeFriendAvatar(arg0->m_steamID);
        CopySteamAvatar(Handle, AvatarTexture);

        if (AvatarRequests.Contains(uint64SteamId))
        {
            FAvatarLoadCallback Callback = AvatarRequests.FindRef(uint64SteamId);
            if (Callback.IsBound())
                Callback.Execute(AvatarTexture);

            AvatarRequests.Remove(uint64SteamId);
        }
    }
    else if (AvatarRequests.Contains(uint64SteamId))
    {
        LoadSteamAvatar(CSteamID(uint64SteamId));
    }
}

void USteamGameInstance::EventGameRichPresenceJoinRequested(GameRichPresenceJoinRequested_t* arg0)
{
    if (SteamRichPresenceJoinRequested.IsBound())
        SteamRichPresenceJoinRequested.Broadcast(STEAMID_TO_INT64(arg0->m_steamIDFriend), FString(arg0->m_rgchConnect));
}

void USteamGameInstance::EventGameLobbyJoinRequested(GameLobbyJoinRequested_t* arg0)
{
    if (SteamLobbyJoinRequested.IsBound())
        SteamLobbyJoinRequested.Broadcast(STEAMID_TO_INT64(arg0->m_steamIDLobby), STEAMID_TO_INT64(arg0->m_steamIDFriend));
}

void USteamGameInstance::EventGameOverlayActivated(GameOverlayActivated_t* arg0)
{
    if (SteamGameOverlayActivated.IsBound())
        SteamGameOverlayActivated.Broadcast(arg0->m_bActive == 1, arg0->m_bUserInitiated);
}

void USteamGameInstance::EventGameConnectedFriendChatMsg(GameConnectedFriendChatMsg_t* arg0)
{
    if (SteamFriendChatMsg.IsBound())
    {
        EChatEntryType ChatEntryType;
        char* Buffer = new char[8193];
        int32 resultLength = SteamFriends()->GetFriendMessage(arg0->m_steamIDUser, arg0->m_iMessageID, Buffer, 8193, &ChatEntryType);

        FString messageText;
        messageText.AppendChars(Buffer, resultLength);

        SteamFriendChatMsg.Broadcast(static_cast<int64>(arg0->m_steamIDUser.ConvertToUint64()), messageText, static_cast<UEChatEntryType>(ChatEntryType));

        delete[] Buffer;
    }
}

void USteamGameInstance::EventFriendRichPresenceUpdate(FriendRichPresenceUpdate_t* arg0)
{
    if (SteamFriendRichPresenceUpdate.IsBound())
        SteamFriendRichPresenceUpdate.Broadcast(static_cast<int64>(arg0->m_steamIDFriend.ConvertToUint64()), static_cast<int32>(arg0->m_nAppID));
}

void USteamGameInstance::EventPersonaStateChange(PersonaStateChange_t* arg0)
{
    if (arg0->m_nChangeFlags & k_EPersonaChangeAvatar)
    {
        if (Avatars.Contains(arg0->m_ulSteamID))
        {
            UTexture2D* AvatarTexture = Avatars.FindRef(arg0->m_ulSteamID);
            int Handle = SteamFriends()->GetLargeFriendAvatar(CSteamID(arg0->m_ulSteamID));
            CopySteamAvatar(Handle, AvatarTexture);

            if (AvatarRequests.Contains(arg0->m_ulSteamID))
            {
                FAvatarLoadCallback Callback = AvatarRequests.FindRef(arg0->m_ulSteamID);
                if (Callback.IsBound())
                    Callback.Execute(AvatarTexture);

                AvatarRequests.Remove(arg0->m_ulSteamID);
            }
        }
        else if (AvatarRequests.Contains(arg0->m_ulSteamID))
        {
            LoadSteamAvatar(CSteamID(arg0->m_ulSteamID));
        }
    }

    if (SteamPersonaStateChange.IsBound())
    {
        UEPersonaChange MappedValue = UEPersonaChange::EPC_None;

        if (arg0->m_nChangeFlags & k_EPersonaChangeName)
            MappedValue = UEPersonaChange::EPC_Name;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeStatus)
            MappedValue = UEPersonaChange::EPC_Status;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeComeOnline)
            MappedValue = UEPersonaChange::EPC_Online;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeGoneOffline)
            MappedValue = UEPersonaChange::EPC_Offline;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeGamePlayed)
            MappedValue = UEPersonaChange::EPC_GamePlayed;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeGameServer)
            MappedValue = UEPersonaChange::EPC_GameServer;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeAvatar)
            MappedValue = UEPersonaChange::EPC_Avatar;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeJoinedSource)
            MappedValue = UEPersonaChange::EPC_JoinedSource;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeLeftSource)
            MappedValue = UEPersonaChange::EPC_LeftSource;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeRelationshipChanged)
            MappedValue = UEPersonaChange::EPC_RelationshipChanged;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeNameFirstSet)
            MappedValue = UEPersonaChange::EPC_NameFirstSet;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeBroadcast)
            MappedValue = UEPersonaChange::EPC_Broadcast;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeNickname)
            MappedValue = UEPersonaChange::EPC_Nickname;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeSteamLevel)
            MappedValue = UEPersonaChange::EPC_SteamLevel;
        else if (arg0->m_nChangeFlags & k_EPersonaChangeRichPresence)
            MappedValue = UEPersonaChange::EPC_RichPresence;

        SteamPersonaStateChange.Broadcast(static_cast<int64>(arg0->m_ulSteamID), MappedValue);
    }
}

void USteamGameInstance::EventSteamInventoryResultReady_t(SteamInventoryResultReady_t* arg0)
{
    if (InventoryRequests.Contains(arg0->m_handle))
    {
        FItemResultRequest resultRequest = InventoryRequests.FindRef(arg0->m_handle);
        TArray<FItemDetailWithProperties> ItemDetailsArray;
        UEResult result = static_cast<UEResult>(arg0->m_result);

        if (arg0->m_result == EResult::k_EResultOK)
        {
            if (!resultRequest.userId.IsValid()
                || SteamInventory()->CheckResultSteamID(arg0->m_handle, resultRequest.userId))
            {
                SteamItemDetails_t* pOutItemsArray = nullptr;
                uint32 punOutItemsArraySize = 0;
                bool bSuccess = SteamInventory()->GetResultItems(arg0->m_handle, pOutItemsArray, &punOutItemsArraySize);
                if (bSuccess)
                {
                    pOutItemsArray = new SteamItemDetails_t[punOutItemsArraySize];
                    bSuccess = SteamInventory()->GetResultItems(arg0->m_handle, pOutItemsArray, &punOutItemsArraySize);

                    if (bSuccess && punOutItemsArraySize > 0)
                    {
                        for (uint32 i = 0; i < punOutItemsArraySize; ++i)
                        {
                            FItemDetailWithProperties ItemDetail = FItemDetailWithProperties();
                            ItemDetail.instanceId = static_cast<int64>(pOutItemsArray[i].m_itemId);
                            ItemDetail.definitionId = static_cast<int32>(pOutItemsArray[i].m_iDefinition);
                            ItemDetail.quantity = static_cast<int32>(pOutItemsArray[i].m_unQuantity);
                            ItemDetail.flags = static_cast<int32>(pOutItemsArray[i].m_unFlags);

                            if (resultRequest.properties.Num() > 0)
                            {
                                for (const FString& prop : resultRequest.properties)
                                {
                                    uint32 bufferSize = 0;
                                    if (SteamInventory()->GetResultItemProperty(arg0->m_handle, i, StringCast<ANSICHAR>(*prop).Get(), NULL, &bufferSize))
                                    {
                                        char* pchValueBuffer = new char[bufferSize];
                                        if (SteamInventory()->GetResultItemProperty(arg0->m_handle, i, StringCast<ANSICHAR>(*prop).Get(), pchValueBuffer, &bufferSize))
                                        {
                                            FKeyValuePair pair = FKeyValuePair();
                                            pair.key = prop;
                                            pair.value = FString(pchValueBuffer);
                                            ItemDetail.properties.Add(pair);
                                        }
                                        delete[] pchValueBuffer;
                                    }
                                }
                            }

                            ItemDetailsArray.Add(ItemDetail);
                        }
                    }

                    delete[] pOutItemsArray;
                }
            }
            else
                result = UEResult::EPC_InvalidSteamID;
        }

        if (resultRequest.callback.IsBound())
            resultRequest.callback.Execute(result, ItemDetailsArray);

        SteamInventory()->DestroyResult(arg0->m_handle);
    }
    else if (SteamInventoryResultReady.IsBound())
    {
        UEResult result = static_cast<UEResult>(arg0->m_result);
        SteamInventoryResultReady.Broadcast(result, arg0->m_handle);
    }
}

void USteamGameInstance::EventSteamInventoryDefinitionUpdate_t(SteamInventoryDefinitionUpdate_t* arg0)
{
    if (SteamInventoryDefinitionUpdate.IsBound())
    {
        uint32 size = 0;
        SteamInventory()->GetItemDefinitionIDs(nullptr, &size);
        if (size > 0)
        {
            TArray<int32> result;
            result.AddUninitialized(static_cast<int32>(size));
            SteamInventory()->GetItemDefinitionIDs(result.GetData(), &size);
            SteamInventoryDefinitionUpdate.Broadcast(result);
        }
    }
}

void USteamGameInstance::EventMicroTxnAuthorizationResponse_t(MicroTxnAuthorizationResponse_t* arg0)
{
    if (SteamMicroTxnAuthorizationResponse.IsBound())
    {
        int32 nAppId = static_cast<int32>(arg0->m_unAppID);
        int64 orderId = static_cast<int64>(arg0->m_ulOrderID);
        bool authorized = arg0->m_bAuthorized == 1;
        SteamMicroTxnAuthorizationResponse.Broadcast(nAppId, orderId, authorized);
    }
}

void USteamGameInstance::EventFavoritesListAccountsUpdated(FavoritesListAccountsUpdated_t* arg0)
{
    if (SteamFavoritesListAccountsUpdated.IsBound())
        SteamFavoritesListAccountsUpdated.Broadcast(static_cast<UEResult>(arg0->m_eResult));
}

void USteamGameInstance::EventFavoritesListChanged(FavoritesListChanged_t* arg0)
{
    if (SteamFavoritesListChanged.IsBound())
        SteamFavoritesListChanged.Broadcast(static_cast<int32>(arg0->m_nAppID), HeathenTools::IPAddress(arg0->m_nIP), static_cast<int32>(arg0->m_nQueryPort), static_cast<int32>(arg0->m_nConnPort), arg0->m_nFlags == 0x01, arg0->m_bAdd);
}

void USteamGameInstance::EventLobbyChatMsg(LobbyChatMsg_t* arg0)
{
    if (SteamLobbyChatMsg.IsBound())
        SteamLobbyChatMsg.Broadcast(static_cast<int64>(arg0->m_ulSteamIDLobby), static_cast<int64>(arg0->m_ulSteamIDUser), static_cast<UEChatEntryType>(arg0->m_eChatEntryType), static_cast<int32>(arg0->m_iChatID));
}

void USteamGameInstance::EventLobbyChatUpdate(LobbyChatUpdate_t* arg0)
{
    if (SteamLobbyChatUpdate.IsBound())
        SteamLobbyChatUpdate.Broadcast(static_cast<int64>(arg0->m_ulSteamIDLobby), static_cast<int64>(arg0->m_ulSteamIDUserChanged), static_cast<int64>(arg0->m_ulSteamIDMakingChange), static_cast<UEChatMemberStateChange>(arg0->m_rgfChatMemberStateChange));
}

void USteamGameInstance::EventLobbyDataUpdate(LobbyDataUpdate_t* arg0)
{
    if (SteamLobbyDataUpdate.IsBound())
        SteamLobbyDataUpdate.Broadcast(static_cast<int64>(arg0->m_ulSteamIDLobby), static_cast<int64>(arg0->m_ulSteamIDMember), arg0->m_bSuccess == 1);
}

void USteamGameInstance::EventLobbyEnter(LobbyEnter_t* arg0)
{
    if (SteamLobbyEnter.IsBound())
        SteamLobbyEnter.Broadcast(static_cast<int64>(arg0->m_ulSteamIDLobby), static_cast<UEChatRoomEnterResponse>(arg0->m_EChatRoomEnterResponse));
}

void USteamGameInstance::EventLobbyGameCreated(LobbyGameCreated_t* arg0)
{
    if (SteamLobbyGameCreated.IsBound())
        SteamLobbyGameCreated.Broadcast(static_cast<int64>(arg0->m_ulSteamIDLobby), static_cast<int64>(arg0->m_ulSteamIDGameServer), HeathenTools::IPAddress(arg0->m_unIP), static_cast<int32>(arg0->m_usPort));
}

void USteamGameInstance::EventLobbyInvite(LobbyInvite_t* arg0)
{
    if (SteamLobbyInvite.IsBound())
        SteamLobbyInvite.Broadcast(static_cast<int64>(arg0->m_ulSteamIDUser), static_cast<int64>(arg0->m_ulSteamIDLobby), static_cast<int64>(arg0->m_ulGameID));
}

void USteamGameInstance::EventGetTicketForWebApiResponse(GetTicketForWebApiResponse_t* arg0)
{
    if (SteamTicketForWebApiResponse.IsBound())
    {
        SteamTicketForWebApiResponse.Broadcast(static_cast<UEResult>(arg0->m_eResult), arg0->m_hAuthTicket, TArray<uint8>(arg0->m_rgubTicket, arg0->m_cubTicket));
    }
}
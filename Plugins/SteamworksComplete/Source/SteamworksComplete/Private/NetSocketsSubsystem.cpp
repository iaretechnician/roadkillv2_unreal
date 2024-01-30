/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*																				 *
* Derived from and inspired by Epic Game's Steam Sockets plugin					 *
*                                                                                *
**********************************************************************************/

#include "NetSocketsSubsystem.h"
#include "Misc/CommandLine.h"
#include "Misc/ConfigCacheIni.h"
#include "NetSocketsTaskManager.h"
#include "NetSocketsNetDriver.h"
#include "SteamNetSocket.h"
#include "NetSocketsTypes.h"

FNetSocketsSubsystem* FNetSocketsSubsystem::SocketSingleton = nullptr;

FNetSocketsSubsystem* FNetSocketsSubsystem::Create()
{
	if (SocketSingleton == nullptr)
	{
		SocketSingleton = new FNetSocketsSubsystem();
	}

	return SocketSingleton;
}

void FNetSocketsSubsystem::Destroy()
{
	if (SocketSingleton != nullptr)
	{
		SocketSingleton->Shutdown();
		delete SocketSingleton;
		SocketSingleton = nullptr;
	}
}

bool FNetSocketsSubsystem::Init(FString& Error)
{
	const bool bIsDedicated = IsRunningDedicatedServer();
	if (GConfig)
	{
		if (!GConfig->GetBool(TEXT("/Script/SteamworksComplete.SteamworksComplete"), TEXT("bAllowP2PPacketRelay"), bUseRelays, GEngineIni))
		{
			UE_CLOG(bIsDedicated, LogSockets, Warning, TEXT("Steamworks Complete Sockets: Missing config value for bAllowP2PPacketRelay, will be unable to determine ping functionality"));
		}
	}

	// Debug commandline support for socket relays. 
#if !UE_BUILD_SHIPPING
	bool bOverrideRelays = false;
	if (FParse::Bool(FCommandLine::Get(), TEXT("SteamSocketsRelays"), bOverrideRelays))
	{
		UE_LOG(LogSockets, Log, TEXT("SteamNetSockets: Set relay setting to %d"), bOverrideRelays);
		bUseRelays = bOverrideRelays;
	}
#endif

	// Hook up the Steam Event Manager so we can get connection updates.
	SteamEventManager = MakeUnique<FNetSocketsTaskManager>(this);

	return true;
}

void FNetSocketsSubsystem::Shutdown()
{
	UE_LOG(LogSockets, Log, TEXT("SteamNetSockets: Cleaning up"));

	// Clean up the internal event manager.
	if (SteamEventManager.IsValid())
	{
		SteamEventManager.Reset();
	}

	// Remove all our listeners that were pending.
	PendingListenerArray.Empty();
}

class FSocket* FNetSocketsSubsystem::CreateSocket(const FName& SocketType, const FString& SocketDescription, const FName& ProtocolType)
{
	FName ProtocolTypeToUse = ProtocolType;
	if (ProtocolTypeToUse.IsNone())
	{
		if (IsUsingRelayNetwork())
		{
			ProtocolTypeToUse = FNetworkProtocolTypes::SteamNetSocketsP2P;
		}
		else
		{
			// In addition to the P2P framework, the Steam Sockets can also work over IP.
			// Generally, it's better to use the UIpNetDriver, but to prevent us from crashing
			// if you have relays disabled, the fallback is the IP framework.
			ProtocolTypeToUse = FNetworkProtocolTypes::SteamNetSocketsIP;
		}
	}
	else if (ProtocolType != FNetworkProtocolTypes::SteamNetSocketsP2P && ProtocolType != FNetworkProtocolTypes::SteamNetSocketsIP)
	{
		UE_LOG(LogSockets, Warning, TEXT("SteamNetSockets: Attempted to create a socket with a protocol outside of the steam network!"));
		return nullptr;
	}

	return static_cast<FSocket*>(new FSteamNetSocket(SOCKTYPE_Streaming, SocketDescription, ProtocolTypeToUse));
}

void FNetSocketsSubsystem::DestroySocket(FSocket* Socket)
{
	if (Socket != nullptr)
	{
		FSteamNetSocket* SteamSocket = static_cast<FSteamNetSocket*>(Socket);
		UE_LOG(LogSockets, Verbose, TEXT("SteamNetSockets: Destroy socket called on %u"), SteamSocket->InternalHandle);

		// Save the internal handle because it will be invalid soon.
		FNetSocketInformation* SocketInfo = GetSocketInfo(SteamSocket->InternalHandle);
		if (SocketInfo != nullptr)
		{
			// Warn the NetDriver that we're destructing the socket such that nothing else tampers with it.
			UNetSocketsNetDriver* SteamNetDriver = SocketInfo->NetDriver.Get();
			if (SteamNetDriver)
			{
				SteamNetDriver->ResetSocketInfo(SteamSocket);
			}
			SocketInfo->Socket = nullptr;
			SocketInfo->MarkForDeletion(); // Mark us for deletion so we're removed from the map
		}

		// Socket closure will properly mark the socket for pending removal.
		Socket->Close();

		delete Socket;
	}
}

FAddressInfoResult FNetSocketsSubsystem::GetAddressInfo(const TCHAR* HostName, const TCHAR* ServiceName,
	EAddressInfoFlags QueryFlags, const FName ProtocolTypeName, ESocketType SocketType)
{
	// We don't support queries but if we can make an address from it then that's good enough.
	FAddressInfoResult ResultData(HostName, ServiceName);

	// Determine if we're asking for any specific protocol.
	const bool bWildcardProtocol = ProtocolTypeName.IsNone();

	// If you requested anything outside of the steam network, we will be unable to support it.
	// This occurs when you have:
	// * Given no hostname and service name information
	// * Given a service name but the service name is not numeric (SteamNetwork can only take port numbers)
	// * Specified a protocol type that's not a part of the steam network.
	if ((HostName == nullptr && ServiceName == nullptr) || (ServiceName != nullptr && !FCString::IsNumeric(ServiceName)) ||
		(ProtocolTypeName != FNetworkProtocolTypes::SteamNetSocketsP2P && ProtocolTypeName != FNetworkProtocolTypes::SteamNetSocketsIP && !bWildcardProtocol))
	{
		ResultData.ReturnCode = SE_EINVAL;
		return ResultData;
	}

	TArray<TSharedPtr<FInternetAddr>> AddressList;
	// Handle binding flags or if you don't pass in a hostname
	if (HostName == nullptr || EnumHasAnyFlags(QueryFlags, EAddressInfoFlags::BindableAddress))
	{
		// Grab all the adapters, we'll process them later.
		// We don't care about the return type as on SteamSockets this call cannot fail.
		GetLocalAdapterAddresses(AddressList);
	}
	else
	{
		TSharedPtr<FInternetAddr> SerializedAddr = GetAddressFromString(HostName);
		// Only add to this list if the serialization succeeded.
		if (SerializedAddr.IsValid())
		{
			AddressList.Add(SerializedAddr);
		}
	}

	if (AddressList.Num() >= 1)
	{
		// Assume we have no error, we'll check for this later.
		ResultData.ReturnCode = SE_NO_ERROR;

		// Preprocess the port information so we can set it later.
		int32 PortToUse = -1;
		if (ServiceName != nullptr && FCString::IsNumeric(ServiceName))
		{
			PortToUse = FCString::Atoi(ServiceName);
		}

		// Process the address list.
		for (auto& AddressItem : AddressList)
		{
			// Check if the types match (add them anyways if protocol type wasn't specified).
			if (bWildcardProtocol || (!bWildcardProtocol && AddressItem->GetProtocolType() == ProtocolTypeName))
			{
				// We will be writing directly into the array but this doesn't matter in our case as all the addresses are shared allocations.
				if (PortToUse >= 0)
				{
					AddressItem->SetPort(PortToUse);
				}
				ResultData.Results.Add(FAddressInfoResultData(AddressItem.ToSharedRef(), 0, AddressItem->GetProtocolType(), SOCKTYPE_Streaming));
			}
		}

		// If our address list is essentially empty due to mismatches, then set the correct error flag.
		if (ResultData.Results.Num() <= 0)
		{
			if (!bWildcardProtocol)
			{
				// We couldn't find addresses that gave what the user was looking for.
				ResultData.ReturnCode = SE_ADDRFAMILY;
			}
			else
			{
				// Otherwise something unforeseen happened.
				ResultData.ReturnCode = SE_EFAULT;
			}
		}
	}
	else
	{
		ResultData.ReturnCode = SE_NO_DATA;
	}

	return ResultData;
}

TSharedPtr<FInternetAddr> FNetSocketsSubsystem::GetAddressFromString(const FString& IPAddress)
{
	TSharedRef<FInternetAddrNetSockets> NewAddr = StaticCastSharedRef<FInternetAddrNetSockets>(CreateInternetAddr());

	// Passing an empty string should just map it to the any address.
	if (IPAddress.IsEmpty())
	{
		// NOTE: There's a lot of questions as to what this address should be here.
		// Should we be returning the identity or the any address, 
		// there's not really a good way to determine default.
		NewAddr->SetAnyAddress();

		return NewAddr;
	}

	bool bIsAddrValid = false;
	NewAddr->SetIp(*IPAddress, bIsAddrValid);
	if (!bIsAddrValid)
	{
		return nullptr;
	}

	return NewAddr;
}

bool FNetSocketsSubsystem::GetHostName(FString& HostName)
{
	// We have no way of looking up addresses on this platform.
	UE_LOG(LogSockets, Warning, TEXT("GetHostName is not supported on SteamNetSockets"));
	return false;
}

TSharedRef<FInternetAddr> FNetSocketsSubsystem::CreateInternetAddr()
{
	return MakeShareable(new FInternetAddrNetSockets());
}

TSharedRef<FInternetAddr> FNetSocketsSubsystem::CreateInternetAddr(const FName RequestedProtocol)
{
	return MakeShareable(new FInternetAddrNetSockets(RequestedProtocol));
}

const TCHAR* FNetSocketsSubsystem::GetSocketAPIName() const
{
	return TEXT("SteamNetSockets");
}

bool FNetSocketsSubsystem::GetLocalAdapterAddresses(TArray<TSharedPtr<FInternetAddr>>& OutAddresses)
{
	// Add the account addresses
	if (IsUsingRelayNetwork())
	{
		TSharedPtr<FInternetAddr> IdentityAddress = GetIdentityAddress();
		if (IdentityAddress.IsValid())
		{
			OutAddresses.Add(IdentityAddress);
		}
	}

	// Always include the any address.
	TSharedRef<FInternetAddr> AnyAddress = CreateInternetAddr(FNetworkProtocolTypes::SteamNetSocketsIP);
	AnyAddress->SetAnyAddress();
	OutAddresses.Add(AnyAddress);

	return true;
}

TArray<TSharedRef<FInternetAddr>> FNetSocketsSubsystem::GetLocalBindAddresses()
{
	TArray<TSharedRef<FInternetAddr>> OutAddresses;
	TArray<TSharedPtr<FInternetAddr>> AdapterAddresses;
	GetLocalAdapterAddresses(AdapterAddresses);

	// Add Multihome
	TSharedRef<FInternetAddr> MultihomeAddress = CreateInternetAddr(FNetworkProtocolTypes::SteamNetSocketsIP);
	if (GetMultihomeAddress(MultihomeAddress))
	{
		OutAddresses.Add(MultihomeAddress);
	}

	// Add all the adapter addresses
	for (const auto& AdapterAddress : AdapterAddresses)
	{
		OutAddresses.Add(AdapterAddress.ToSharedRef());
	}

	return OutAddresses;
}

void FNetSocketsSubsystem::CleanSocketInformation(bool bForceClean)
{
	for (SocketHandleInfoMap::TIterator It(SocketInformationMap); It; ++It)
	{
		FNetSocketInformation& SocketInfo = It.Value();
		if (SocketInfo.IsMarkedForDeletion() || bForceClean)
		{
			// Close the netdriver and any connections.
			UNetSocketsNetDriver* NetDriverInfo = SocketInfo.NetDriver.Get();
			if (NetDriverInfo && SocketInfo.Parent == nullptr)
			{
				NetDriverInfo->Shutdown();
				SocketInfo.NetDriver.Reset();
			}

			// Close the socket (frees internal memory)
			//
			// Deletion with SteamSockets is fairly safe. Multiple paths can mark a socket 
			// for deletion but it will only ever be deleted once.
			if (SocketInfo.Socket)
			{
				DestroySocket(SocketInfo.Socket);
			}

			It.RemoveCurrent();
		}
	}
}

void FNetSocketsSubsystem::DumpSocketInformationMap() const
{
#if !UE_BUILD_SHIPPING
	if (SocketInformationMap.Num() < 1)
	{
		UE_LOG(LogSockets, Log, TEXT("SteamNetSockets: Socket Information Map is empty!"));
		return;
	}

	UE_LOG(LogSockets, Log, TEXT("SteamNetSockets: Printing Socket Information Map:\n"));
	for (SocketHandleInfoMap::TConstIterator It(SocketInformationMap); It; ++It)
	{
		const FNetSocketInformation& SocketInfo = It.Value();
		UE_LOG(LogSockets, Log, TEXT("# %s"), *SocketInfo.ToString());
	}
#endif
}

bool FNetSocketsSubsystem::Tick(float DeltaTime)
{
	QUICK_SCOPE_CYCLE_COUNTER(STAT_FSteamNetSocketsSubsystem_Tick);

	// Handle all of our updates from the Steam API callbacks
	if (SteamEventManager.IsValid())
	{
		SteamEventManager->Tick();
	}

	CleanSocketInformation(false);
	return true;
}

bool FNetSocketsSubsystem::Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	bool bIsHandled = false;

#if !UE_BUILD_SHIPPING
	if (FParse::Command(&Cmd, TEXT("PrintSteamNetSocketInfo")))
	{
		bIsHandled = true;
		DumpSocketInformationMap();
	}
	if (FParse::Command(&Cmd, TEXT("PrintPendingSteamNetSocketInfo")))
	{
		bIsHandled = true;
		UE_LOG(LogSockets, Log, TEXT("SteamNetSockets: Printing Pending Socket Info"));
		for (auto& Pending : PendingListenerArray)
		{
			UE_LOG(LogSockets, Log, TEXT("# %s"), *Pending.ToString());
		}
	}
	else if (FParse::Command(&Cmd, TEXT("ClearSteamSocketInfo")))
	{
		UE_LOG(LogSockets, Log, TEXT("SteamNetSockets: Clearing all socket information!"));
		bIsHandled = true;
		CleanSocketInformation(true);
	}
	else if (FParse::Command(&Cmd, TEXT("TogglePeekMessaging")))
	{
		bIsHandled = true;
		bShouldTestPeek = !bShouldTestPeek;
		UE_LOG(LogSockets, Log, TEXT("SteamNetSockets: Set Peek Messaging to %d"), bShouldTestPeek);
	}
#endif

	return bIsHandled;
}

ISteamNetworkingSockets* FNetSocketsSubsystem::GetSteamSocketsInterface()
{
	return (SteamGameServerNetworkingSockets() != nullptr && IsRunningDedicatedServer()) ? SteamGameServerNetworkingSockets() : SteamNetworkingSockets();
}

bool FNetSocketsSubsystem::FNetSocketInformation::operator==(const FInternetAddr& InAddr) const
{
	FInternetAddrNetSockets SteamAddr = *((FInternetAddrNetSockets*)&InAddr);
	return *Addr == SteamAddr;
}

void FNetSocketsSubsystem::FNetSocketInformation::MarkForDeletion()
{
	bMarkedForDeletion = true;
	UNetSocketsNetDriver* NetDriverObj = NetDriver.Get();
	if (Socket != nullptr && NetDriverObj)
	{
		UE_LOG(LogSockets, Verbose, TEXT("SteamNetSockets: Handle %u marked for deletion. Has parent? %d NetDriver definition: %s"),
			Socket->InternalHandle, (Parent != nullptr), *NetDriverObj->GetDescription());
	}
}

FString FNetSocketsSubsystem::FNetSocketInformation::ToString() const
{
	return FString::Printf(TEXT("SocketInfo: Addr[%s], Socket[%u], Status[%d], Listener[%d], HasNetDriver[%d], MarkedForDeletion[%d]"),
		(Addr.IsValid() ? *Addr->ToString(true) : TEXT("INVALID")), (Socket != nullptr ? Socket->InternalHandle : 0),
		(Socket != nullptr ? (int32)Socket->GetConnectionState() : -1),
		(Parent != nullptr),
		NetDriver.IsValid(), bMarkedForDeletion);
}

FString FNetSocketsSubsystem::FSteamPendingSocketInformation::ToString() const
{
	UNetSocketsNetDriver* SteamNetDriver = NetDriver.Get();
	return FString::Printf(TEXT("PendingSocketInfo: NetDriver name %s"), (SteamNetDriver != nullptr) ? *SteamNetDriver->GetDescription() : TEXT("INVALID"));
}

FNetSocketsSubsystem::FNetSocketInformation* FNetSocketsSubsystem::GetSocketInfo(SteamNetSocketHandles InternalSocketHandle)
{
	if (InternalSocketHandle == k_HSteamNetConnection_Invalid)
	{
		UE_LOG(LogSockets, Warning, TEXT("SteamNetSockets: Cannot get information on an invalid socket handle, returning null"));
		return nullptr;
	}

	return SocketInformationMap.Find(InternalSocketHandle);
}

FNetSocketsSubsystem::FNetSocketInformation* FNetSocketsSubsystem::GetSocketInfo(const FInternetAddr& ForAddress)
{
	// Small time save with invalid data
	if (!ForAddress.IsValid())
	{
		return nullptr;
	}

	// This functionality is a bit slower, because we have to do complete address lookups through the map.
	// It's much better to use the handles to find the socket info. There's also a possibility for collisions
	// here as well. This is why internally, this function is not used as much (only when we don't have enough data).
	for (SocketHandleInfoMap::TIterator It(SocketInformationMap); It; ++It)
	{
		if (It.Value() == ForAddress)
		{
			return &(It.Value());
		}
	}

	return nullptr;
}

void FNetSocketsSubsystem::AddSocket(const FInternetAddr& ForAddr, FSteamNetSocket* NewSocket, FSteamNetSocket* ParentSocket)
{
	if (NewSocket == nullptr || ForAddr.IsValid() == false)
	{
		UE_LOG(LogSockets, Warning, TEXT("SteamNetSockets: Attempted to track invalid socket data!"));
		return;
	}

	STEAM_SDK_IGNORE_REDUNDANCY_START
		// Do not attempt to add socket data for sockets that have invalid handles.
		if (NewSocket->InternalHandle == k_HSteamListenSocket_Invalid || NewSocket->InternalHandle == k_HSteamNetConnection_Invalid)
		{
			UE_LOG(LogSockets, Verbose, TEXT("SteamNetSockets: Dropped socket tracking for socket with invalid handle."));
			return;
		}
	STEAM_SDK_IGNORE_REDUNDANCY_END

		FNetSocketInformation NewSocketInfo(ForAddr.Clone(), NewSocket, ParentSocket);
	if (SocketInformationMap.Find(NewSocket->InternalHandle) == nullptr)
	{
		UE_LOG(LogSockets, Log, TEXT("SteamNetSockets: Now tracking socket %u for addr %s, has parent? %d"), NewSocket->InternalHandle, *ForAddr.ToString(true), (ParentSocket != nullptr));
		SocketInformationMap.Add(NewSocket->InternalHandle, NewSocketInfo);
	}
	else
	{
		UE_LOG(LogSockets, Warning, TEXT("SteamNetSockets: A socket with the address %s already exists, will not add another!"), *ForAddr.ToString(true));
	}
}

void FNetSocketsSubsystem::RemoveSocketsForListener(FSteamNetSocket* ListenerSocket)
{
	if (ListenerSocket == nullptr)
	{
		UE_LOG(LogSockets, Warning, TEXT("SteamNetSockets: Attempted to free sockets attached to an invalid listener!"));
		return;
	}

	// When a listen socket closes in the SteamAPI, all sockets that were created from that listener are ungracefully closed.
	// However this does not free any memory, nor does it alert any other object. As such, we have to process this destruction here.
	UE_LOG(LogSockets, Log, TEXT("SteamNetSockets: Closing all sockets attached to listener %u"), ListenerSocket->InternalHandle);
	for (SocketHandleInfoMap::TIterator It(SocketInformationMap); It; ++It)
	{
		FNetSocketInformation& SocketInfo = It.Value();
		if (SocketInfo.Parent == ListenerSocket)
		{
			UE_LOG(LogSockets, Verbose, TEXT("SteamNetSockets: Removed socket %u"), SocketInfo.Socket->InternalHandle);
			// A loop later will clean out these connections, shutting them down properly.
			// This is to prevent acting on deletion events from the API.
			SocketInfo.Parent = nullptr;
			// The children also should not be cleaning up any netdrivers they don't own
			SocketInfo.NetDriver.Reset();
			SocketInfo.MarkForDeletion();
		}
	}
}

void FNetSocketsSubsystem::QueueRemoval(SteamNetSocketHandles RemoveHandle)
{
	FNetSocketInformation* SocketInfo = GetSocketInfo(RemoveHandle);
	if (SocketInfo != nullptr)
	{
		FString Address = (SocketInfo->Addr.IsValid()) ? SocketInfo->Addr->ToString(true) : TEXT("INVALID");
		UE_LOG(LogSockets, Verbose, TEXT("SteamNetSockets: Marked socket %u with address %s for removal (pending)"), RemoveHandle, *Address);
		SocketInfo->MarkForDeletion();
	}
}

void FNetSocketsSubsystem::LinkNetDriver(FSocket* Socket, UNetSocketsNetDriver* NewNetDriver)
{
	if (Socket == nullptr || NewNetDriver == nullptr)
	{
		UE_LOG(LogSockets, Warning, TEXT("SteamNetSockets: Attempted to create an invalid socket/netdriver pairing!"));
		return;
	}

	FSteamNetSocket* SteamSocket = static_cast<FSteamNetSocket*>(Socket);
	// Do not attempt to track netdrivers with invalid sockets.
	STEAM_SDK_IGNORE_REDUNDANCY_START
		if (SteamSocket->InternalHandle == k_HSteamListenSocket_Invalid || SteamSocket->InternalHandle == k_HSteamNetConnection_Invalid)
		{
			UE_LOG(LogSockets, Verbose, TEXT("SteamNetSockets: Dropped netdriver link with socket that has invalid handle."));
			return;
		}
	STEAM_SDK_IGNORE_REDUNDANCY_END

		// Link the netdriver to this socket.
		FNetSocketInformation* SocketInfo = GetSocketInfo(SteamSocket->InternalHandle);
	if (SocketInfo != nullptr)
	{
		SocketInfo->NetDriver = NewNetDriver;
	}
}

void FNetSocketsSubsystem::AddDelayedListener(FSteamNetSocket* ListenSocket, UNetSocketsNetDriver* NewNetDriver)
{
	// Ignore any invalid data.
	if (ListenSocket == nullptr || NewNetDriver == nullptr)
	{
		UE_LOG(LogSockets, Warning, TEXT("SteamNetSockets: Add delayed listener cannot listen to invalid data!"));
		return;
	}

	FSteamPendingSocketInformation WatchedListener;
	WatchedListener.Socket = ListenSocket;

	// Track the netdriver
	WatchedListener.NetDriver = NewNetDriver;

	// Push to the array list
	PendingListenerArray.Add(WatchedListener);
}

void FNetSocketsSubsystem::EventSteamGameServerConnectFailure(SteamServerConnectFailure_t* arg0)
{
	OnServerLoginComplete(false);
}

void FNetSocketsSubsystem::EventSteamGameServersConnected(SteamServersConnected_t* arg0)
{
	OnServerLoginComplete(true);
}

void FNetSocketsSubsystem::EventSteamGameServersDisconnected(SteamServersDisconnected_t* arg0)
{
	OnServerLoginComplete(false);
}

void FNetSocketsSubsystem::OnServerLoginComplete(bool bWasSuccessful)
{
	TSharedPtr<FInternetAddrNetSockets> SteamIdentityAddr = StaticCastSharedPtr<FInternetAddrNetSockets>(GetIdentityAddress());
	for (auto& DelayedListener : PendingListenerArray)
	{
		UNetSocketsNetDriver* SteamNetDriver = DelayedListener.NetDriver.Get();
		if (SteamNetDriver == nullptr)
		{
			continue;
		}

		// If we are not successful, we need to get out.
		if (!bWasSuccessful)
		{
			// Mark the NetDriver for cleanup.
			SteamNetDriver->Shutdown();
			continue;
		}

		// Update the binding address
		if (SteamIdentityAddr.IsValid())
		{
			// Push in the new valid information.
			int32 ListenPort = DelayedListener.Socket->BindAddress.GetPlatformPort();
			TSharedPtr<FInternetAddrNetSockets> ListenerAddress = StaticCastSharedRef<FInternetAddrNetSockets>(SteamIdentityAddr->Clone());
			ListenerAddress->SetPlatformPort(ListenPort);

			// Update the socket and netdriver addresses
			DelayedListener.Socket->BindAddress = *ListenerAddress;
			SteamNetDriver->LocalAddr = ListenerAddress;

			// Actually start the listen call
			if (!DelayedListener.Socket->Listen(0))
			{
				// Connection failed, fire shutdown.
				SteamNetDriver->Shutdown();
				continue;
			}
		}

		// Link the netdriver information so that we're ready.
		LinkNetDriver(DelayedListener.Socket, SteamNetDriver);

		// Toggle off the netdriver's tick throttle.
		SteamNetDriver->bIsDelayedNetworkAccess = false;

		UE_LOG(LogSockets, Verbose, TEXT("SteamNetSockets: Processed delayed listener: %s"), *DelayedListener.ToString());
	}

	PendingListenerArray.Empty();
}

TSharedPtr<FInternetAddr> FNetSocketsSubsystem::GetIdentityAddress()
{
	SteamNetworkingIdentity SteamIDData;
	TSharedRef<FInternetAddrNetSockets> SteamAddr = StaticCastSharedRef<FInternetAddrNetSockets>(CreateInternetAddr(FNetworkProtocolTypes::SteamNetSocketsP2P));

	// Attempt to get the machine's identity (their steam ID)
	if (GetSteamSocketsInterface() && GetSteamSocketsInterface()->GetIdentity(&SteamIDData))
	{
		// If we got it, assign it to the SteamAddr
		SteamAddr->Addr = SteamIDData;
		return SteamAddr;
	}
	else if (!IsRunningDedicatedServer() && SteamUser())
	{
		CSteamID CurrentUser = SteamUser()->GetSteamID();
		if (CurrentUser.IsValid())
		{
			SteamAddr->Addr.SetSteamID(CurrentUser);
		}
		else
		{
			UE_LOG(LogSockets, Warning, TEXT("SteamNetSockets: Unable to process current user's steam id!"));
		}
	}

	return nullptr;
}

bool FNetSocketsSubsystem::IsLoggedInToSteam() const
{
	// Check the game server functionality first because servers should be prioritized over SteamUser calls if
	// the server has linked with the client dlls via launch arguments
	return ((SteamGameServer() && IsRunningDedicatedServer()) ? SteamGameServer()->BLoggedOn() : ((SteamUser()) ? SteamUser()->BLoggedOn() : false));
}

// Helper function to translate Valve connection state messages into something human readable.
static FString ConnectionStateToString(const ESteamNetworkingConnectionState& ConnectionState)
{
	switch (ConnectionState)
	{
	default:
	case k_ESteamNetworkingConnectionState_None:
		return TEXT("None");
	case k_ESteamNetworkingConnectionState_Connecting:
		return TEXT("Connecting");
	case k_ESteamNetworkingConnectionState_FindingRoute:
		return TEXT("Finding Route");
	case k_ESteamNetworkingConnectionState_Connected:
		return TEXT("Connected");
	case k_ESteamNetworkingConnectionState_ClosedByPeer:
		return TEXT("Closed by Peer");
	case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
		return TEXT("Local Connection Issue");
	}

	return TEXT("");
}

void FNetSocketsSubsystem::NetSocketEventHandler(struct SteamNetConnectionStatusChangedCallback_t* Message)
{
	if (Message == nullptr || FNetSocketsSubsystem::GetSteamSocketsInterface() == nullptr)
	{
		return;
	}

	// New Connection came in
	if (Message->m_eOldState == k_ESteamNetworkingConnectionState_None &&
		Message->m_info.m_eState == k_ESteamNetworkingConnectionState_Connecting &&
		Message->m_info.m_hListenSocket != k_HSteamListenSocket_Invalid)
	{
		// Push this to the netdriver.
		FNetSocketInformation* SocketInfo = GetSocketInfo(Message->m_info.m_hListenSocket);
		UNetSocketsNetDriver* NetDriver = SocketInfo->NetDriver.Get();
		if (SocketInfo && NetDriver)
		{
			NetDriver->OnConnectionCreated(Message->m_info.m_hListenSocket, Message->m_hConn);
		}
	}
	else if ((Message->m_eOldState == k_ESteamNetworkingConnectionState_Connecting || Message->m_eOldState == k_ESteamNetworkingConnectionState_FindingRoute) &&
		Message->m_info.m_eState == k_ESteamNetworkingConnectionState_Connected)
	{
		// Connection has been established
		FNetSocketInformation* SocketInfo = GetSocketInfo(Message->m_hConn);
		UNetSocketsNetDriver* NetDriver = SocketInfo->NetDriver.Get();
		if (SocketInfo && NetDriver)
		{
			NetDriver->OnConnectionUpdated(Message->m_hConn, (int32)Message->m_info.m_eState);
		}
	}
	else if ((Message->m_eOldState == k_ESteamNetworkingConnectionState_FindingRoute || Message->m_eOldState == k_ESteamNetworkingConnectionState_Connecting
		|| Message->m_eOldState == k_ESteamNetworkingConnectionState_Connected) &&
		(Message->m_info.m_eState == k_ESteamNetworkingConnectionState_ClosedByPeer || Message->m_info.m_eState == k_ESteamNetworkingConnectionState_ProblemDetectedLocally))
	{
		FNetSocketInformation* SocketInfo = GetSocketInfo(Message->m_hConn);
		// If we know anything about the socket, we need to act on it.
		if (SocketInfo != nullptr && SocketInfo->Socket != nullptr && !SocketInfo->Socket->bIsListenSocket)
		{
			// Print out information about the disconnection.
			UE_LOG(LogSockets, Verbose, TEXT("SteamNetSockets: Connection %u has disconnected. Old state: %s Reason: %s"), Message->m_hConn,
				*ConnectionStateToString(Message->m_eOldState), *ConnectionStateToString(Message->m_info.m_eState));

			// Let the netdriver know of the disconnection
			UNetSocketsNetDriver* NetDriver = SocketInfo->NetDriver.Get();
			if (NetDriver)
			{
				NetDriver->OnConnectionDisconnected(Message->m_hConn);
			}

			// If we close via the listen socket deletion then let the connection just die.
			// Do not alert anyone as we already know we're disconnected.
			//
			// To determine this case we follow these two rules:
			// * Servers will always mark all related sockets as not having a parent.
			// * Clients will never have valid listener socket handles in their message data.
			if (SocketInfo->Parent != nullptr || Message->m_info.m_hListenSocket == k_HSteamListenSocket_Invalid)
			{
				// If we are in here, it means we are a genuine disconnection, not caused by the listen socket destruction
				// Or any other event that we ourselves caused.
				//
				// Mark this to be cleaned up. The tick will handle actual deletion.
				SocketInfo->MarkForDeletion();
			}
		}
	}
}
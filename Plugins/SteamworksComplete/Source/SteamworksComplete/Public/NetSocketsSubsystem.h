/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*																				 *
* Derived from and inspired by Epic Game's Steam Sockets plugin					 *
*                                                                                *
**********************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "Containers/Ticker.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "UObject/WeakObjectPtr.h"
#include "NetSocketsPackage.h"
#include "NetSocketsTaskManagerInterface.h"
#include "NetSocketsTypes.h"
#include "SocketTypes.h"
#include "./Runtime/Launch/Resources/Version.h"
THIRD_PARTY_INCLUDES_START
#include <HeathenTools.h>
THIRD_PARTY_INCLUDES_END

// Forward declare some internal types for bookkeeping.
class FSteamNetSocket;
class UNetSocketsNetDriver;

class STEAMWORKSCOMPLETE_API FNetSocketsSubsystem : public ISocketSubsystem, public FTSTickerObjectBase, public FSelfRegisteringExec
{
public:

	FNetSocketsSubsystem() :
		LastSocketError(0),
		bShouldTestPeek(false),
		SteamEventManager(nullptr),
		bUseRelays(true)
	{
	}

	//~ Begin SocketSubsystem Interface
	virtual bool Init(FString& Error) override;
	virtual void Shutdown() override;

	virtual class FSocket* CreateSocket(const FName& SocketType, const FString& SocketDescription, const FName& ProtocolType) override;
	virtual void DestroySocket(class FSocket* Socket) override;

	virtual FAddressInfoResult GetAddressInfo(const TCHAR* HostName, const TCHAR* ServiceName = nullptr,
		EAddressInfoFlags QueryFlags = EAddressInfoFlags::Default,
		const FName ProtocolTypeName = NAME_None,
		ESocketType SocketType = ESocketType::SOCKTYPE_Unknown) override;
	virtual TSharedPtr<FInternetAddr> GetAddressFromString(const FString& IPAddress) override;

	virtual bool GetHostName(FString& HostName) override;

	virtual TSharedRef<FInternetAddr> CreateInternetAddr() override;
	virtual TSharedRef<FInternetAddr> CreateInternetAddr(const FName RequestedProtocol) override;

	virtual const TCHAR* GetSocketAPIName() const override;

	virtual ESocketErrors GetLastErrorCode() override { return (ESocketErrors)LastSocketError; }
	virtual ESocketErrors TranslateErrorCode(int32 Code) override { return (ESocketErrors)Code; }

	virtual bool GetLocalAdapterAddresses(TArray<TSharedPtr<FInternetAddr>>& OutAddresses) override;
	virtual TArray<TSharedRef<FInternetAddr>> GetLocalBindAddresses() override;
	virtual bool HasNetworkDevice() override { return true; }
	virtual bool IsSocketWaitSupported() const override { return false; }
	virtual bool RequiresChatDataBeSeparate() override { return false; }
	virtual bool RequiresEncryptedPackets() override { return false; }
	//~ End SocketSubsystem Interface

	//~ Begin FTickerObject Interface
	virtual bool Tick(float DeltaTime) override;
	//~ End FTickerObject Interface

	//~ Begin FSelfRegisteringExec Interface
	virtual bool Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	//~ End FSelfRegisteringExec Interface

	/** Returns if the application is using the SteamSocket relays */
	bool IsUsingRelayNetwork() const { return bUseRelays; }

	/** Basic function to determine if Steam has been initialized properly. */
	bool IsSteamInitialized() const { return SteamGameServer() || SteamUser(); }

	static class ISteamNetworkingSockets* GetSteamSocketsInterface();

	//HACK: Unreal 5.1 isn't playing nice with this so force it to public on 5.1 and older
#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 2
	PACKAGE_SCOPE:
#endif
	/** A struct for holding steam socket information and managing bookkeeping on the protocol. */
	struct FNetSocketInformation
	{
		FNetSocketInformation(TSharedPtr<FInternetAddr> InAddr, FSteamNetSocket* InSocket, FSteamNetSocket* InParent = nullptr) :
			Addr(InAddr),
			Socket(InSocket),
			Parent(InParent),
			NetDriver(nullptr),
			bMarkedForDeletion(false)
		{
		}

		void MarkForDeletion();

		bool IsMarkedForDeletion() const { return bMarkedForDeletion; }

		bool operator==(const FSteamNetSocket* RHS) const
		{
			return Socket == RHS;
		}

		bool operator==(const FInternetAddr& InAddr) const;

		bool operator==(const FNetSocketInformation& RHS) const
		{
			return RHS.Addr == Addr && RHS.Socket == Socket;
		}

		bool IsValid() const
		{
			return Addr.IsValid() && Parent != nullptr;
		}

		FString ToString() const;

		TSharedPtr<FInternetAddr> Addr;
		FSteamNetSocket* Socket;
		// Sockets created from a listener have a parent
		FSteamNetSocket* Parent;
		// The NetDriver for this connection.
		TWeakObjectPtr<UNetSocketsNetDriver> NetDriver;
	private:
		bool bMarkedForDeletion;
	};

	// Steam socket queriers
	FNetSocketInformation* GetSocketInfo(SteamNetSocketHandles InternalSocketHandle);
	FNetSocketInformation* GetSocketInfo(const FInternetAddr& ForAddress);

	// Steam socket bookkeeping modifiers
	void AddSocket(const FInternetAddr& ForAddr, FSteamNetSocket* NewSocket, FSteamNetSocket* ParentSocket = nullptr);
	void RemoveSocketsForListener(FSteamNetSocket* ListenerSocket);
	void QueueRemoval(SteamNetSocketHandles SocketHandle);
	void LinkNetDriver(FSocket* Socket, UNetSocketsNetDriver* NewNetDriver);

	// Delayed listen socket helpers.
	void AddDelayedListener(FSteamNetSocket* ListenSocket, UNetSocketsNetDriver* NewNetDriver);
	void OnServerLoginComplete(bool bWasSuccessful);

	// Returns this machine's identity in the form of a FInternetAddrNetSockets
	TSharedPtr<FInternetAddr> GetIdentityAddress();

	// Returns if our account is currently logged into the Steam network
	bool IsLoggedInToSteam() const;

	/** Last error set by the socket subsystem or one of its sockets */
	int32 LastSocketError;

	// Singleton helpers
	static FNetSocketsSubsystem* Create();
	static void Destroy();

	// SteamAPI internals handler
	void NetSocketEventHandler(struct SteamNetConnectionStatusChangedCallback_t* ConnectionEvent);

	/** Flag for testing peek messaging (only usable in non-shipping builds) */
	bool bShouldTestPeek;

protected:
	void CleanSocketInformation(bool bForceClean);
	void DumpSocketInformationMap() const;

	/** Single instantiation of this subsystem */
	static FNetSocketsSubsystem* SocketSingleton;

	/** Event manager for Steam tasks */
	TUniquePtr<class FNetSocketsTaskManagerInterface> SteamEventManager;

	/** Determines if the connections are going to be using the relay network */
	bool bUseRelays;

	/** Active connection bookkeeping */
	typedef TMap<SteamNetSocketHandles, FNetSocketInformation> SocketHandleInfoMap;
	SocketHandleInfoMap SocketInformationMap;

	/** Structure for handling sockets that cannot be established due to platform login (for listener sockets) */
	struct FSteamPendingSocketInformation
	{
		FSteamNetSocket* Socket;
		TWeakObjectPtr<UNetSocketsNetDriver> NetDriver;

		FString ToString() const;
	};

	// Array of listeners we need to activate.
	TArray<FSteamPendingSocketInformation> PendingListenerArray;

	// Delegate handle for handling when a dedicated server logs into the Steam platform
	FDelegateHandle SteamServerLoginDelegateHandle;

private:
	//STEAM_GAMESERVER_CALLBACK
	STEAM_GAMESERVER_CALLBACK(FNetSocketsSubsystem, EventSteamGameServerConnectFailure, SteamServerConnectFailure_t);
	STEAM_GAMESERVER_CALLBACK(FNetSocketsSubsystem, EventSteamGameServersConnected, SteamServersConnected_t);
	STEAM_GAMESERVER_CALLBACK(FNetSocketsSubsystem, EventSteamGameServersDisconnected, SteamServersDisconnected_t);
};
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
#include "UObject/ObjectMacros.h"
#include "Engine/NetDriver.h"
#include "NetSocketsPackage.h"
#include "NetSocketsTypes.h"
#include "NetSocketsNetDriver.generated.h"

class FNetworkNotify;

UCLASS(transient, config = Engine)
class STEAMWORKSCOMPLETE_API UNetSocketsNetDriver : public UNetDriver
{
	GENERATED_BODY()

public:

	UNetSocketsNetDriver() :
		Socket(nullptr),
		bIsDelayedNetworkAccess(false)
	{
	}

	//~ Begin UObject Interface
	virtual void PostInitProperties() override;
	//~ End UObject Interface

	//~ Begin UNetDriver Interface.
	virtual void Shutdown() override;
	virtual bool IsAvailable() const override;
	virtual bool InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error) override;
	virtual bool InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error) override;
	virtual bool InitListen(FNetworkNotify* InNotify, FURL& LocalURL, bool bReuseAddressAndPort, FString& Error) override;
	virtual void TickDispatch(float DeltaTime) override;
	virtual void LowLevelSend(TSharedPtr<const FInternetAddr> Address, void* Data, int32 CountBits, FOutPacketTraits& Traits) override;
	virtual void LowLevelDestroy() override;
	virtual class ISocketSubsystem* GetSocketSubsystem() override;
	virtual bool IsNetResourceValid(void) override;
	//~ End UNetDriver Interface

	bool ArePacketHandlersDisabled() const;

protected:
	class FSteamNetSocket* Socket;
	bool bIsDelayedNetworkAccess;

	void ResetSocketInfo(const class FSteamNetSocket* RemovedSocket);

	UNetConnection* FindClientConnectionForHandle(SteamNetSocketHandles SocketHandle);

	void OnConnectionCreated(SteamNetSocketHandles ListenParentHandle, SteamNetSocketHandles SocketHandle);
	void OnConnectionUpdated(SteamNetSocketHandles SocketHandle, int32 NewState);
	void OnConnectionDisconnected(SteamNetSocketHandles SocketHandle);

	friend class FNetSocketsSubsystem;
};
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
#include "Engine/NetConnection.h"
#include "SteamNetSocket.h"
#include "NetSocketsNetConnection.generated.h"

// Forward declare some types for functional type arguments
//class FSocket;
//class UNetDriver;
//class FInternetAddr;
//class FSteamNetSocket;

UCLASS(transient, config = Engine)
class STEAMWORKSCOMPLETE_API UNetSocketsNetConnection : public UNetConnection
{
	GENERATED_BODY()

public:

	UNetSocketsNetConnection() :
		ConnectionSocket(nullptr),
		bInConnectionlessHandshake(false)
	{
	}

	//~ Begin NetConnection Interface
	virtual void CleanUp() override;
	virtual void InitBase(UNetDriver* InDriver, FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void InitRemoteConnection(UNetDriver* InDriver, FSocket* InSocket, const FURL& InURL, const FInternetAddr& InRemoteAddr,
		EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void InitLocalConnection(UNetDriver* InDriver, FSocket* InSocket, const FURL& InURL, EConnectionState InState, int32 InMaxPacket = 0, int32 InPacketOverhead = 0) override;
	virtual void LowLevelSend(void* Data, int32 CountBits, FOutPacketTraits& Traits) override;
	FString LowLevelGetRemoteAddress(bool bAppendPort = false) override;
	FString LowLevelDescribe() override;
	//~ End NetConnection Interface

private:
	const FSteamNetSocket* GetRawSocket() const { return ConnectionSocket; }

	void HandleRecvMessage(void* InData, int32 SizeOfData, const FInternetAddr* InFormattedAddress);

	void FlagForHandshake() { bInConnectionlessHandshake = true; }

	void ClearSocket() { ConnectionSocket = nullptr; }

	FSteamNetSocket* ConnectionSocket;
	bool bInConnectionlessHandshake;

	friend class UNetSocketsNetDriver;
};
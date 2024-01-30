/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*																				 *
* Derived from and inspired by Epic Game's Steam Sockets plugin					 *
*                                                                                *
**********************************************************************************/

#pragma once

#include "NetSocketsTaskManagerInterface.h"
#include "NetSocketsPrivate.h"
#include "Containers/Queue.h"

// A glorified event manager for handling status updates about SteamSockets we hold.
class FNetSocketsTaskManager : public FNetSocketsTaskManagerInterface
{
public:
	FNetSocketsTaskManager(FNetSocketsSubsystem* SocketSub) :
		FNetSocketsTaskManagerInterface(SocketSub),
		OnConnectionStatusChangedCallback(this, &FNetSocketsTaskManager::OnConnectionStatusChanged),
		OnConnectionStatusChangedGSCallback(this, &FNetSocketsTaskManager::OnConnectionStatusChangedGS)
	{

	}

	virtual ~FNetSocketsTaskManager()
	{
		MessageQueue.Empty();
	}

	virtual void Tick() override;


private:
	STEAM_CALLBACK(FNetSocketsTaskManager, OnConnectionStatusChanged, SteamNetConnectionStatusChangedCallback_t, OnConnectionStatusChangedCallback);
	STEAM_GAMESERVER_CALLBACK(FNetSocketsTaskManager, OnConnectionStatusChangedGS, SteamNetConnectionStatusChangedCallback_t, OnConnectionStatusChangedGSCallback);

	// A copied queue of our messages we get from the SteamAPI. There's no difference between client and server messages
	// just how they are registered.
	TQueue<SteamNetConnectionStatusChangedCallback_t> MessageQueue;
};
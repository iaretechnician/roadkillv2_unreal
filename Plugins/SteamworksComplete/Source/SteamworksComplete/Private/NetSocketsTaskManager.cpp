/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*																				 *
* Derived from and inspired by Epic Game's Steam Sockets plugin					 *
*                                                                                *
**********************************************************************************/

#include "NetSocketsTaskManager.h"
#include "NetSocketsSubsystem.h"

void FNetSocketsTaskManager::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* SteamConnectionMessage)
{
	// When we are called in here, we are likely going to be on the Steam OSS's online thread.
	// The Steam callback API will fire on whatever thread the OSS is on.
	//
	// Because of how the callback api works, we need to store copies of these messages, as the pointers will be invalid
	// the instant we leave our function.
	MessageQueue.Enqueue(*SteamConnectionMessage);
}

void FNetSocketsTaskManager::OnConnectionStatusChangedGS(SteamNetConnectionStatusChangedCallback_t* SteamConnectionMessage)
{
	MessageQueue.Enqueue(*SteamConnectionMessage);
}

void FNetSocketsTaskManager::Tick()
{
	SteamNetConnectionStatusChangedCallback_t CallbackData;
	while (!MessageQueue.IsEmpty())
	{
		FMemory::Memzero(CallbackData);
		if (SocketSubsystem && MessageQueue.Dequeue(CallbackData))
		{
			SocketSubsystem->NetSocketEventHandler(&CallbackData);
		}
		else
		{
			break;
		}
	}
}
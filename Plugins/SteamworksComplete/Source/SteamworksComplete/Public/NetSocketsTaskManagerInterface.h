/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*																				 *
* Derived from and inspired by Epic Game's Steam Sockets plugin					 *
*                                                                                *
**********************************************************************************/

#pragma once

class FNetSocketsTaskManagerInterface
{
public:
	FNetSocketsTaskManagerInterface(class FNetSocketsSubsystem* SocketSub) :
		SocketSubsystem(SocketSub)
	{
	}

	virtual ~FNetSocketsTaskManagerInterface() = default;

	/** A function that will be called on every update tick of the SteamSocketsSubsystem */
	virtual void Tick() = 0;

protected:
	class FNetSocketsSubsystem* SocketSubsystem;
};
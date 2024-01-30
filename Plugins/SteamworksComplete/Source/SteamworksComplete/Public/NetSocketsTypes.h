/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*																				 *
* Derived from and inspired by Epic Game's Steam Sockets plugin					 *
*                                                                                *
**********************************************************************************/

#pragma once

#include "Engine/EngineBaseTypes.h"

#ifndef STEAM_NETWORKING_SOCKETS_SUBSYSTEM
#define STEAM_NETWORKING_SOCKETS_SUBSYSTEM FName(TEXT("SteamNetSockets"))
#endif

// Add our two new protocol types
namespace FNetworkProtocolTypes
{
	STEAMWORKSCOMPLETE_API extern const FLazyName SteamNetSocketsP2P;
	STEAMWORKSCOMPLETE_API extern const FLazyName SteamNetSocketsIP;
}

typedef uint32 SteamNetSocketHandles;
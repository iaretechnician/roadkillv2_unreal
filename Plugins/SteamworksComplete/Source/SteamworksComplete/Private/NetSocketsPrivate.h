/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*																				 *
* Derived from and inspired by Epic Game's Steam Sockets plugin					 *
*                                                                                *
**********************************************************************************/

#pragma once

//#include "CoreMinimal.h"
#include "HAL/Platform.h"

// To make less changes to the Steam OSS, we'll define this here
// This will allow us to easily snap into the Steam OSS code
#ifndef STEAM_URL_PREFIX
#define STEAM_URL_PREFIX TEXT("steam.")
#endif

THIRD_PARTY_INCLUDES_START
#include <../sdk/steam_api.h>
#include <../sdk/steam_gameserver.h>
#include <../sdk/isteamnetworkingsockets.h>
#include <../sdk/isteamnetworkingutils.h>
#include <../sdk/steamnetworkingtypes.h>
THIRD_PARTY_INCLUDES_END

/** Some flags in the Steam SDK share similar numeral values which could change in the future.
 *  Static analysis does not understand the difference and instead sees code like Object == 1 || Object == 1
 *  For safety, we need to check both constants as the values in the SDK could change in a future version.
 *  So this macro block was introduced to silence value redundancy warnings in static analysis.
 */
#ifdef _MSC_VER
#define STEAM_SDK_IGNORE_REDUNDANCY_START \
		__pragma(warning(push)) \
		__pragma(warning(disable: 6287)) 

#define STEAM_SDK_IGNORE_REDUNDANCY_END \
		__pragma(warning(pop))
#else
#define STEAM_SDK_IGNORE_REDUNDANCY_START
#define STEAM_SDK_IGNORE_REDUNDANCY_END
#endif

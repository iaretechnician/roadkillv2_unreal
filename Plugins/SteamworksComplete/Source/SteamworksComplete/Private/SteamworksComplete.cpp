/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#include "SteamworksComplete.h"
#include "NetSocketsSubsystem.h"
#include "SocketSubsystemModule.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/Paths.h"
#include "HAL/PlatformProcess.h"
#include "Misc/ConfigCacheIni.h"

#define LOCTEXT_NAMESPACE "FSteamworksCompleteModule"

namespace FNetworkProtocolTypes
{
	const FLazyName SteamNetSocketsP2P(TEXT("SteamNetSocketsP2P"));
	const FLazyName SteamNetSocketsIP(TEXT("SteamNetSocketsIP"));
}

void FSteamworksCompleteModule::StartupModule()
{
	const bool bIsNotEditor = (IsRunningDedicatedServer() || IsRunningGame());

	// Settings flags
	bool bOverrideSocketSubsystem = true;

	// Use this flag from the SteamNetworking configuration so that the IPNetDrivers can be used as needed.
	if (GConfig)
	{
		GConfig->GetBool(TEXT("/Script/SteamworksComplete.SteamworksComplete"), TEXT("bUseSteamNetworking"), bOverrideSocketSubsystem, GEngineIni);
	}

	// Create and register our singleton factory with the main online subsystem for easy access
	FNetSocketsSubsystem* SocketSubsystem = FNetSocketsSubsystem::Create();
	FString Error;
	if (SocketSubsystem->Init(Error))
	{
		// Register our socket Subsystem
		FSocketSubsystemModule& SSS = FModuleManager::LoadModuleChecked<FSocketSubsystemModule>("Sockets");
		SSS.RegisterSocketSubsystem(STEAM_NETWORKING_SOCKETS_SUBSYSTEM, SocketSubsystem, bOverrideSocketSubsystem);
	}
	else
	{
		UE_LOG(LogSockets, Error, TEXT("SteamNetSockets: Could not initialize SteamSockets, got error: %s"), *Error);
		FNetSocketsSubsystem::Destroy();
	}
}

void FSteamworksCompleteModule::ShutdownModule()
{
	FModuleManager& ModuleManager = FModuleManager::Get();

	if (ModuleManager.IsModuleLoaded("Sockets"))
	{
		FSocketSubsystemModule& SSS = FModuleManager::GetModuleChecked<FSocketSubsystemModule>("Sockets");
		SSS.UnregisterSocketSubsystem(STEAM_NETWORKING_SOCKETS_SUBSYSTEM);
	}
	FNetSocketsSubsystem::Destroy();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSteamworksCompleteModule, SteamworksComplete)

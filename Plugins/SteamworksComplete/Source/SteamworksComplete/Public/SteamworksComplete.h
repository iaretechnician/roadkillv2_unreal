/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

#pragma once

#include "Modules/ModuleManager.h"

class FSteamworksCompleteModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	/** Handle to the test dll we will load */
	void*	ExampleLibraryHandle;
};

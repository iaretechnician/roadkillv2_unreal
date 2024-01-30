// Copyright Alex Quevillon. All Rights Reserved.

using UnrealBuildTool;

public class UtuPlugin : ModuleRules {
	public UtuPlugin(ReadOnlyTargetRules Target) : base(Target) {
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"UnrealEd",
				"ToolMenus",
				"LevelEditor",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
				"UMG",
				"UMGEditor",
				"Blutility",
				"Json",
				"JsonUtilities",
				"AssetTools",
				"AssetRegistry",
				"KismetCompiler",
				"Kismet",
				"BlueprintGraph",
				"CinematicCamera",
				"DesktopPlatform",
				"MaterialEditor",
				"ContentBrowser",
			}
			);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}

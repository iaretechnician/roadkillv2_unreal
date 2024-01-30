/*********************************************************************************
*                                                                                *
* Copyright © 2023 by Heathen Engineering Limited, an Irish registered company   *
* # 556277, VAT IE3394133CH, contact Heathen via support@heathen.group           *
*                                                                                *
**********************************************************************************/

using System.IO;
using UnrealBuildTool;

public class SteamworksComplete : ModuleRules
{
	public SteamworksComplete(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDefinitions.Add("STEAMWORKSCOMPLETE_MODULE=1");

        if (Target.Platform != UnrealTargetPlatform.Android && Target.Platform != UnrealTargetPlatform.IOS)
		{
			PublicIncludePaths.AddRange(new string[]
			{
				Path.Combine(ModuleDirectory, "../ThirdParty/sdk"),
				Path.Combine(ModuleDirectory, "Public")
            });

			PublicDependencyModuleNames.AddRange(new string[]
			{
				"Core",
				"CoreUObject",
                "NetCore",
                "Engine",
                "Sockets",
                "PacketHandler",
                "Projects",
                "JsonUtilities"
            });

            DirectoryInfo dir = new DirectoryInfo(ModuleDirectory);
            dir = dir.Parent.Parent;
            string binDirPath = Path.Combine(dir.FullName, "Binaries", Target.Platform.ToString());
            if (!Directory.Exists(binDirPath))
                Directory.CreateDirectory(binDirPath);

            // Add the Steamworks SDK libraries based on the target platform
            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                // Add the Steamworks DLLs as runtime dependencies for Windows
                string SdkEncryptedAppTicketFolder = Path.Combine(ModuleDirectory, "..", "ThirdParty", "SteamworksCompleteLibrary", "sdk", "public", "steam", "lib", "win64");
                string RedistributableBinFolder = Path.Combine(ModuleDirectory, "..", "ThirdParty", "SteamworksCompleteLibrary", "sdk", "redistributable_bin", "win64");
                PublicAdditionalLibraries.Add(Path.Combine(SdkEncryptedAppTicketFolder, "sdkencryptedappticket64.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(RedistributableBinFolder, "steam_api64.lib"));

                RuntimeDependencies.Add(Path.Combine(SdkEncryptedAppTicketFolder, "sdkencryptedappticket64.dll"), StagedFileType.UFS);
                RuntimeDependencies.Add(Path.Combine(RedistributableBinFolder, "steam_api64.dll"), StagedFileType.UFS);

                //If its not already there ... add it
                if (!File.Exists(Path.Combine(binDirPath, "steam_api64.dll")))
                {
                    File.Copy(Path.Combine(SdkEncryptedAppTicketFolder, "sdkencryptedappticket64.dll"), Path.Combine(binDirPath, "sdkencryptedappticket64.dll"), true);
                    File.Copy(Path.Combine(RedistributableBinFolder, "steam_api64.dll"), Path.Combine(binDirPath, "steam_api64.dll"), true);
                }
            }
            else if (Target.Platform == UnrealTargetPlatform.Linux)
            {
                string SdkEncryptedAppTicketFolder = Path.Combine(ModuleDirectory, "..", "ThirdParty", "SteamworksCompleteLibrary", "sdk", "public", "steam", "lib", "linux64");
                string RedistributableBinFolder = Path.Combine(ModuleDirectory, "..", "ThirdParty", "SteamworksCompleteLibrary", "sdk", "redistributable_bin", "linux64");

                // Add the Steamworks DLLs as runtime dependencies for Linux
                PublicAdditionalLibraries.Add(Path.Combine(SdkEncryptedAppTicketFolder, "libsdkencryptedappticket.so"));
                PublicAdditionalLibraries.Add(Path.Combine(RedistributableBinFolder, "libsteam_api.so"));

                RuntimeDependencies.Add(Path.Combine(SdkEncryptedAppTicketFolder, "libsdkencryptedappticket.so"), StagedFileType.UFS);
                RuntimeDependencies.Add(Path.Combine(RedistributableBinFolder, "libsteam_api.so"), StagedFileType.UFS);

                //If its not already there ... add it
                if (!File.Exists(Path.Combine(binDirPath, "libsteam_api.so")))
                {
                    File.Copy(Path.Combine(SdkEncryptedAppTicketFolder, "libsdkencryptedappticket.so"), Path.Combine(binDirPath, "libsdkencryptedappticket.so"), true);
                    File.Copy(Path.Combine(RedistributableBinFolder, "libsteam_api.so"), Path.Combine(binDirPath, "libsteam_api.so"), true);
                }
            }
            else if (Target.Platform == UnrealTargetPlatform.Mac)
            {
                string SdkEncryptedAppTicketFolder = Path.Combine(ModuleDirectory, "..", "ThirdParty", "SteamworksCompleteLibrary", "sdk", "public", "steam", "lib", "osx");
                string RedistributableBinFolder = Path.Combine(ModuleDirectory, "..", "ThirdParty", "SteamworksCompleteLibrary", "sdk", "redistributable_bin", "osx");

                // Add the Steamworks DLLs as runtime dependencies for Mac
                PublicAdditionalLibraries.Add(Path.Combine(SdkEncryptedAppTicketFolder, "libsdkencryptedappticket.dylib"));
                PublicAdditionalLibraries.Add(Path.Combine(RedistributableBinFolder, "libsteam_api.dylib"));

                RuntimeDependencies.Add(Path.Combine(SdkEncryptedAppTicketFolder, "libsdkencryptedappticket.dylib"), StagedFileType.UFS);
                RuntimeDependencies.Add(Path.Combine(RedistributableBinFolder, "libsteam_api.dylib"), StagedFileType.UFS);

                //If its not already there ... add it
                if (!File.Exists(Path.Combine(binDirPath, "libsteam_api.dylib")))
                {
                    File.Copy(Path.Combine(SdkEncryptedAppTicketFolder, "libsdkencryptedappticket.dylib"), Path.Combine(binDirPath, "libsdkencryptedappticket.dylib"), true);
                    File.Copy(Path.Combine(RedistributableBinFolder, "libsteam_api.dylib"), Path.Combine(binDirPath, "libsteam_api.dylib"), true);
                }
            }

        }
    }
}

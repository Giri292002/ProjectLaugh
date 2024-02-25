// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectLaugh : ModuleRules
{
	public ProjectLaugh(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "LevelSequence", "OnlineSubsystem", "OnlineSubsystemUtils", "MovieScene", "Niagara" });
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemSteam", "OnlineSubsystemEOS" ,"OnlineSubsystemNull", "AdvancedSessions", "AdvancedSteamSessions" });

       // PublicIncludePaths.AddRange(new string[] { "AdvancedSessions/Public", "AdvancedSessions/Classes", "AdvancedSteamSessions/Public", "AdvancedSteamSessions/Classes" });
    }
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectLaugh : ModuleRules
{
	public ProjectLaugh(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemSteam", "OnlineSubsystemEOS" ,"OnlineSubsystemNull", "AdvancedSessions", "AdvancedSteamSessions", "OnlineSubsystemUtils" });

       // PublicIncludePaths.AddRange(new string[] { "AdvancedSessions/Public", "AdvancedSessions/Classes", "AdvancedSteamSessions/Public", "AdvancedSteamSessions/Classes" });
    }
}

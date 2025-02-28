// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_Urban : ModuleRules
{
	public Project_Urban(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}

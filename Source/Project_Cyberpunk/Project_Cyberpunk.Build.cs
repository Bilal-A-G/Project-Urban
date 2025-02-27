// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_Cyberpunk : ModuleRules
{
	public Project_Cyberpunk(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}

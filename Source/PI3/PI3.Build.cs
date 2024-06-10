// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PI3 : ModuleRules
{
	public PI3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "HeadMountedDisplay", "NavigationSystem" });
	}
}

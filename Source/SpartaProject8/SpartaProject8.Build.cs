// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SpartaProject8 : ModuleRules
{
	public SpartaProject8(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "GameplayAbilities", "GameplayTags", "GameplayTasks" });
	}
}

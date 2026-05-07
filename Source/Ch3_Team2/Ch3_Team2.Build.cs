// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Ch3_Team2 : ModuleRules
{
	public Ch3_Team2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "CommonUI", "ModelViewViewModel" });
		PublicIncludePaths.Add(ModuleDirectory);
	}
}

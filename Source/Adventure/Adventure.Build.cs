// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Adventure : ModuleRules
{
	public Adventure(ReadOnlyTargetRules Target) : base(Target)	
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
		new string[]
		{
			"Core",
			"DeveloperSettings", 
			"Engine",
			"GameplayTags",
			"NetCore",
		}
		);
			
		
		PrivateDependencyModuleNames.AddRange(
		new string[]
		{
			"CoreUObject",
			"UMG",
			"Slate",
			"SlateCore",
			"MessageLog",
		}
		);
	}
}

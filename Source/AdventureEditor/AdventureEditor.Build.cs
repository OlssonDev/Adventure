// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AdventureEditor : ModuleRules
{
	public AdventureEditor(ReadOnlyTargetRules Target) : base(Target)	
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"AssetRegistry",
				"AssetTools",
				"PropertyEditor",
				"ContentBrowser",
				"MessageLog",
			}
		);

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "EditorStyle", 
                "PlacementMode"
			}
			);


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
	            "SlateCore",
	            "Slate",
	            "CoreUObject",
	            "ApplicationCore",
	            "Engine",
	            "RenderCore",
	            "InputCore",
	            "ClassViewer",
	            "Adventure",
	            "AssetTools",
	            "UnrealEd",
	            "Kismet",
	            "AdventureGraphEditor"
            }
            );
	}
}

using UnrealBuildTool;

public class AdventureGraphEditor : ModuleRules
{
    public AdventureGraphEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "Kismet",
                "SlateCore", 
                "Adventure",
                "KismetCompiler",
                "UnrealEd",
                "BlueprintGraph",
                "GraphEditor",
				"SlateCore",
				"ToolMenus",
				"EditorStyle", 
                "AdventureTasks"
            }
        );
    }
}
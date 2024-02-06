using UnrealBuildTool;

public class AdventureTasksEditor : ModuleRules
{
    public AdventureTasksEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core", 
                "UnrealEd",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "AdventureTasks",
                "Adventure",
                "GraphEditor",
                "BlueprintGraph", 
                "AdventureGraphEditor",
                "ToolMenus",
                "KismetCompiler"
            }
        );
    }
}
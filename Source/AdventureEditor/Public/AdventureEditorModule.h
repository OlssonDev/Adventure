// YourModuleNameModule.h
#pragma once

#include "CoreMinimal.h"
#include "IPlacementModeModule.h"
#include "Modules/ModuleInterface.h"

class UAdventureAssetBlueprint;
class IAssetTools;
class IAssetTypeActions;

class FAdventureEditorModule : public IModuleInterface
{
    virtual void ShutdownModule() override;
    virtual void StartupModule() override;
    
    void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action);
    
    void RegisterPlacementModeExtensions();

    public:

    TArray<TSharedPtr<IAssetTypeActions>> CreatedAssetTypeActions;
    
    static uint32 AdventureCategory;
    
    FPlacementCategoryInfo PlacementCategoryInfo = FPlacementCategoryInfo(NSLOCTEXT("PlacementModeAdventure", "Adventure", "Adventure"), "Adventure", TEXT("PMAdventure"), 25);

    #if WITH_UNREAL_DEVELOPER_TOOLS
    static void OnPostWorldCreation(UWorld* InWorld);
    static void OnWorldCleanUp(UWorld*, bool, bool);
    FDelegateHandle OnPostWorldCreationHandle;
    FDelegateHandle OnWorldCleanUpHandle;
    #endif // WITH_UNREAL_DEVELOPER_TOOLS
    
};

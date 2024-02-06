
#define LOCTEXT_NAMESPACE "FAdventureGraphEditorModule"

#include "AdventureGraphEditorModule.h"
#include "AdventureGraphEditor.h"
#include "EdGraphUtilities.h"
#include "FAdventureEditorCommands.h"
#include "ObjectiveDetailCustomization.h"
#include "AdventureBlueprintCompiler.h"
#include "BlueprintAssets/AdventureAssetBlueprint.h"

class FAdventureGraphEditorModule : public IAdventureGraphEditorModule
{
	
	public:
	    
	FAdventureBlueprintCompiler AdventureBlueprintCompiler;
	TSharedPtr<FExtensibilityManager> MenuExtensibilityManager;
	TSharedPtr<FExtensibilityManager> ToolBarExtensibilityManager;
	TSharedPtr<class FGraphPanelNodeFactory_AdventureGraph> GraphPanelNodeFactory_AdventureGraph = nullptr;
    
	virtual void StartupModule() override
	{
		FKismetCompilerContext::RegisterCompilerForBP(UAdventureAssetBlueprint::StaticClass(), [](UBlueprint* InBlueprint, FCompilerResultsLog& InMessageLog, const FKismetCompilerOptions& InCompileOptions)
		{
		return MakeShared<FAdventureCompilerContext>(CastChecked<UAdventureAssetBlueprint>(InBlueprint), InMessageLog, InCompileOptions);
		});

		IKismetCompilerInterface& KismetCompilerModule = FModuleManager::LoadModuleChecked<IKismetCompilerInterface>("KismetCompiler");
		KismetCompilerModule.GetCompilers().Add(&AdventureBlueprintCompiler);

		FAdventureEditorCommands::Register();
		MenuExtensibilityManager = MakeShareable(new FExtensibilityManager);
		ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);

	
		// Register the details customizer
		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomPropertyTypeLayout("AdventureObjectiveEntry", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FObjectiveDetailCustomization::MakeInstance));
	};
	
	virtual void ShutdownModule() override
	{
		
	}
	
	virtual void CreateAdventureEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UAdventureAssetBlueprint* AdventureAsset) override
	{
		TSharedRef<IAdventureGraphEditor> NewAdventureEditor(new IAdventureGraphEditor());
		NewAdventureEditor->InitAdventureEditor(Mode, InitToolkitHost, AdventureAsset);
	}
	
    
	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override { return MenuExtensibilityManager; }
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override { return ToolBarExtensibilityManager; }
	
};

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FAdventureGraphEditorModule, AdventureGraphEditor)
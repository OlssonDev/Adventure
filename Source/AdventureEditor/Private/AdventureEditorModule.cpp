#include "AdventureEditorModule.h"
#include "AdventureMacros.h"
#include "AdventureMessageLogEditor.h"
#include "AdventureStyle.h"
#include "MessageLogInitializationOptions.h"
#include "MessageLogModule.h"
#include "ActorFactories/ActorFactoryBlueprint.h"
#include "Adventure/AdventureLogChannels.h"
#include "AssetTools/FAssetTypeActions_AdventureAsset.h"
#include "AssetTools/FAssetTypeActions_AdventureSaveObject.h"
#include "Factory/AdventureAssetActorFactory.h"
#include "Logging/MessageLog.h"
#include "Volumes/AdventureObjectiveTriggerVolume.h"
#include "Volumes/AdventureStartTriggerVolume.h"

#define LOCTEXT_NAMESPACE "FAdventureEditorModule"

uint32 FAdventureEditorModule::AdventureCategory;

void FAdventureEditorModule::StartupModule()
{
	FAdventureStyle::Initialize();
	
	{
		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AdventureCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Adventure")), LOCTEXT("AdventureCategory", "Adventure"));
		RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_AdventureAsset()));
		RegisterAssetTypeAction(AssetTools, MakeShareable(new FAssetTypeActions_AdventureSaveObject()));
	}
	
	RegisterPlacementModeExtensions();

	//Register the Adventure message log tab
	{
		FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
		FMessageLogInitializationOptions InitOptions;
		InitOptions.bShowPages = true;
		InitOptions.bAllowClear = true;
		InitOptions.bShowInLogWindow = true;
		MessageLogModule.RegisterLogListing("Adventure", LOCTEXT("Adventure", "Adventure"), InitOptions);
		OnPostWorldCreationHandle = FWorldDelegates::OnPostWorldCreation.AddStatic(&FAdventureEditorModule::OnPostWorldCreation);
		OnWorldCleanUpHandle = FWorldDelegates::OnWorldCleanup.AddStatic(&FAdventureEditorModule::OnWorldCleanUp);

		//Register the message log editor
		{
			FAdventureLogMessages::LogMessageDelegate.AddStatic(&AdventureMessageLogEditor::LogMessage);
		}
	}
}

void FAdventureEditorModule::RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
{
	AssetTools.RegisterAssetTypeActions(Action);
	CreatedAssetTypeActions.Add(Action);
}

void FAdventureEditorModule::RegisterPlacementModeExtensions()
{
	if (GEditor)
	{
		IPlacementModeModule& PlacementModeModule = IPlacementModeModule::Get();
		PlacementModeModule.RegisterPlacementCategory(PlacementCategoryInfo);
		
		PlacementModeModule.RegisterPlaceableItem(PlacementCategoryInfo.UniqueHandle, MakeShared<FPlaceableItem>(
		*UActorFactoryBlueprint::StaticClass(),
		AAdventureObjectiveTriggerVolume::StaticClass(),
		NAME_None,
		NAME_None,
		TOptional<FLinearColor>(),
		TOptional<int32>(),
		NSLOCTEXT("PlacementMode", "Trigger Objective Volume", "Objective Trigger Volume")
		));

		PlacementModeModule.RegisterPlaceableItem(PlacementCategoryInfo.UniqueHandle, MakeShared<FPlaceableItem>(
		*UActorFactoryBlueprint::StaticClass(),
		AAdventureStartTriggerVolume::StaticClass(),
		NAME_None,
		NAME_None,
		TOptional<FLinearColor>(),
		TOptional<int32>(),
		NSLOCTEXT("PlacementMode", "Start Adventure Volume", "Start Adventure Volume")
		));
		
		PlacementModeModule.RegenerateItemsForCategory(FBuiltInPlacementCategories::AllClasses());

		UActorFactory* NewFactory = NewObject<UActorFactory>(GetTransientPackage(), UAdventureAssetActorFactory::StaticClass());
		GEditor->ActorFactories.Add(NewFactory);
	}
}

void FAdventureEditorModule::OnPostWorldCreation(UWorld* InWorld)
{
	// clearing out messages from the world being cleaned up
	ADVENTURE_MESSAGELOG().NewPage(FText::FromString("AdventureSession_" + FString(__TIME__)));
}

void FAdventureEditorModule::OnWorldCleanUp(UWorld*, bool, bool)
{
	//Open the Adventure message log if there are warning messages
	ADVENTURE_MESSAGELOG().Open(EMessageSeverity::Info, false);
}

void FAdventureEditorModule::ShutdownModule()
{
	FAdventureStyle::Shutdown();

	if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		// Unregister our custom created assets from the AssetTools
		IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		for (int32 i = 0; i < CreatedAssetTypeActions.Num(); ++i)
		{
			AssetTools.UnregisterAssetTypeActions(CreatedAssetTypeActions[i].ToSharedRef());
		}
	}

	CreatedAssetTypeActions.Empty();
}

IMPLEMENT_MODULE(FAdventureEditorModule, AdventureEditor);

#undef LOCTEXT_NAMESPACE

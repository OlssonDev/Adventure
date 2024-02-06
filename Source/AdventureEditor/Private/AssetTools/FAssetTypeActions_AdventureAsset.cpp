// Fill out your copyright notice in the Description page of Project Settings.


#include "FAssetTypeActions_AdventureAsset.h"

#include "AdventureAsset.h"
#include "AdventureGraphEditorModule.h"
#include "BlueprintAssets/AdventureAssetBlueprint.h"
#include "Factory/AdventureAssetFactory.h"

#define LOCTEXT_NAMESPACE "FAssetTypeActions_AdventureAsset"

FAssetTypeActions_AdventureAsset::FAssetTypeActions_AdventureAsset()
{
	
}

FText FAssetTypeActions_AdventureAsset::GetName() const
{
	return NSLOCTEXT("AssetTypeActions_AdventureAsset", "AssetTypeActions_AdventureAsset", "Adventure Asset");
}

UClass* FAssetTypeActions_AdventureAsset::GetSupportedClass() const
{
	return UAdventureAssetBlueprint::StaticClass();
}

void FAssetTypeActions_AdventureAsset::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	if (IAdventureGraphEditorModule* AdventureEditorModule = &FModuleManager::LoadModuleChecked<IAdventureGraphEditorModule>("AdventureGraphEditor"))
	{
		for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
		{
			AdventureEditorModule->CreateAdventureEditor(EditWithinLevelEditor.IsValid() ? EToolkitMode::WorldCentric : EToolkitMode::Standalone, EditWithinLevelEditor, CastChecked<UAdventureAssetBlueprint>(*ObjIt));
		}
	}
}

UFactory* FAssetTypeActions_AdventureAsset::GetFactoryForBlueprintType(UBlueprint* InBlueprint) const
{
	UAdventureAssetFactory* AdventureAssetFactory = NewObject<UAdventureAssetFactory>();
	AdventureAssetFactory->ParentClass = TSubclassOf<UAdventureAsset>(*InBlueprint->GeneratedClass);
	return AdventureAssetFactory;
}

#undef LOCTEXT_NAMESPACE

// Fill out your copyright notice in the Description page of Project Settings.


#include "FAssetTypeActions_AdventureSaveObject.h"
#include "AdventureSaveObject.h"
#include "BlueprintAssets/AdventureSaveObjectBlueprint.h"
#include "Factory/AdventureSaveObjectFactory.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions_AdventureSaveObject"

FAssetTypeActions_AdventureSaveObject::FAssetTypeActions_AdventureSaveObject()
{
}

FText FAssetTypeActions_AdventureSaveObject::GetName() const
{
	return LOCTEXT("AssetTypeActions_AdventureSaveObject_Name", "Custom Save Object");
}

UClass* FAssetTypeActions_AdventureSaveObject::GetSupportedClass() const
{
	return UAdventureSaveObjectBlueprint::StaticClass();
}

const TArray<FText>& FAssetTypeActions_AdventureSaveObject::GetSubMenus() const
{
	static const TArray<FText> SubMenus
	{
		NSLOCTEXT("AssetTypeActions", "AssetTypeActions_AdventureSaveObject", "Save")
	};

	return SubMenus;
}

UFactory* FAssetTypeActions_AdventureSaveObject::GetFactoryForBlueprintType(UBlueprint* InBlueprint) const
{
	UAdventureSaveObjectFactory* AdventureAssetFactory = NewObject<UAdventureSaveObjectFactory>();
	AdventureAssetFactory->ParentClass = TSubclassOf<UAdventureSaveObject>(*InBlueprint->GeneratedClass);
	return AdventureAssetFactory;
}

#undef LOCTEXT_NAMESPACE

// Fill out your copyright notice in the Description page of Project Settings.

#include "Factory/AdventureAssetFactory.h"
#include "BlueprintEditorSettings.h"
#include "BlueprintAssets/AdventureAssetBlueprint.h"
#include "AdventureAsset.h"
#include "AdventureBlueprintGeneratedClass.h"
#include "AdventureClassPicker.h"
#include "AdventurePluginSettings.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Widgets/Layout/SUniformGridPanel.h"

#define LOCTEXT_NAMESPACE "AdventureAssetFactory"

UAdventureAssetFactory::UAdventureAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UAdventureAssetBlueprint::StaticClass();
	ParentClass = UAdventureAsset::StaticClass();
}

UObject* UAdventureAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, EBlueprintType::BPTYPE_Normal, UAdventureAssetBlueprint::StaticClass(), UAdventureBlueprintGeneratedClass::StaticClass(), CallingContext);
}

UObject* UAdventureAssetFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn, NAME_None);
}

bool UAdventureAssetFactory::ConfigureProperties()
{
	if (auto NewClass = GetDefault<UAdventurePluginSettings>()->DefaultAdventureClass)
	{
		ParentClass = NewClass;
	}
	
	TSharedRef<SAdventureClassPicker> Dialog = SNew(SAdventureClassPicker);
	return Dialog->ConfigureProperties(this);
}

bool UAdventureAssetFactory::CanCreateNew() const
{
	return true;
}

FString UAdventureAssetFactory::GetDefaultNewAssetName() const
{
	return FString(TEXT("ADV_NewAdventure"));
}

FText UAdventureAssetFactory::GetToolTip() const
{
	return LOCTEXT("Adventure Tooltip", "Adventure provides a intuitive, node-based interface to create both simple and complex missions and quests.");
}

FText UAdventureAssetFactory::GetDisplayName() const
{
	return LOCTEXT("Adventure Text", "Adventure");
}

#undef LOCTEXT_NAMESPACE
// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureSaveObjectFactory.h"

#include "AdventureSaveObject.h"
#include "BlueprintAssets/AdventureSaveObjectBlueprint.h"
#include "Kismet2/KismetEditorUtilities.h"

#define LOCTEXT_NAMESPACE "AdventureSaveObjectFactory"

UAdventureSaveObjectFactory::UAdventureSaveObjectFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bCreateNew = true;
	bEditAfterNew = true;
	#if ENGINE_MAJOR_VERSION == 5
	bSkipClassPicker = true;
	#endif
	SupportedClass = UAdventureSaveObjectBlueprint::StaticClass();
	ParentClass = UAdventureSaveObject::StaticClass();
}

bool UAdventureSaveObjectFactory::ConfigureProperties()
{
	#if ENGINE_MAJOR_VERSION == 4
	return true;
	#endif
	return Super::ConfigureProperties();
}

UObject* UAdventureSaveObjectFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	return FKismetEditorUtilities::CreateBlueprint(ParentClass, InParent, Name, EBlueprintType::BPTYPE_Normal, UAdventureSaveObjectBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass(), CallingContext);
}

UObject* UAdventureSaveObjectFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return Super::FactoryCreateNew(Class, InParent, Name, Flags, Context, Warn);
}

FText UAdventureSaveObjectFactory::GetToolTip() const
{
	return LOCTEXT("AdventureSaveObjectFactory_Tooltip", "Create a custom save object class. Assign the class in Project Settings -> Adventure Settings -> Default Save Object Class.");
}

FText UAdventureSaveObjectFactory::GetDisplayName() const
{
	return LOCTEXT("AdventureSaveObjectFactory_Name", "Custom Save Object");
}

#undef LOCTEXT_NAMESPACE

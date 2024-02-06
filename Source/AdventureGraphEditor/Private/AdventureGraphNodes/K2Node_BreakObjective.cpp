// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGraphNodes/K2Node_BreakObjective.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "AdventurePluginSettings.h"
#include "Library/AdventureBlueprintLibrary.h"

UK2Node_BreakObjective::UK2Node_BreakObjective()
{
	FunctionReference.SetFromField<UFunction>(UAdventureBlueprintLibrary::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureBlueprintLibrary, BreakObjectiveWithName)), false);
}

void UK2Node_BreakObjective::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
	
	if (UAdventurePluginSettings::IsUsingGameplayTags())
	{
		FunctionReference.SetFromField<UFunction>(UAdventureBlueprintLibrary::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureBlueprintLibrary, BreakObjectiveWithTag)), false);
	}
	else
	{
		FunctionReference.SetFromField<UFunction>(UAdventureBlueprintLibrary::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureBlueprintLibrary, BreakObjectiveWithName)), false);
	}

	ReconstructNode();
}

FSlateIcon UK2Node_BreakObjective::GetIconAndTint(FLinearColor& OutColor) const
{
	#if ENGINE_MAJOR_VERSION == 4
	static const FSlateIcon Icon = FSlateIcon("EditorStyle", "ClassIcon.StringTable");
	return Icon;
	#endif

	#if ENGINE_MAJOR_VERSION == 5
	static const FSlateIcon Icon = FSlateIcon("EditorStyle", "ClassIcon.StringTable");
	return Icon;
	#endif
}

void UK2Node_BreakObjective::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	Super::GetMenuActions(ActionRegistrar);

	const UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

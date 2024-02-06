// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGraphNodes/K2Node_AdventureState.h"
#include "AdventureGraphSchema.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"

#define LOCTEXT_NAMESPACE "UK2Node_AdventureState"

FSlateIcon UK2Node_AdventureState::GetIconAndTint(FLinearColor& OutColor) const
{
	return FSlateIcon("EditorStyle", "ClassIcon.SlateBrushAsset");
}

FText UK2Node_AdventureState::GetMenuCategory() const
{
	return LOCTEXT("K2Node_AdventureState_Category", "Adventure Graph | Tasks");
}

bool UK2Node_AdventureState::CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const
{
	return Schema->GetClass() == UAdventureGraphSchema::StaticClass();
}

void UK2Node_AdventureState::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

#undef LOCTEXT_NAMESPACE

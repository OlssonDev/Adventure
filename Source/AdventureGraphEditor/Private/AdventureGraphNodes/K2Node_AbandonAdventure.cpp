// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGraphNodes/K2Node_AbandonAdventure.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "AdventureAsset.h"

#define LOCTEXT_NAMESPACE "UK2Node_AbandonAdventure"

UK2Node_AbandonAdventure::UK2Node_AbandonAdventure()
{
	FunctionReference.SetFromField<UFunction>(UAdventureAsset::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureAsset, K2_AbandonAdventure)), true);
}

FLinearColor UK2Node_AbandonAdventure::GetNodeTitleColor() const
{
	return FLinearColor::Gray;
}

#undef LOCTEXT_NAMESPACE

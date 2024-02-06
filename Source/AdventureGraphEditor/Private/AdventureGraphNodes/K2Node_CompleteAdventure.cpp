// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGraphNodes/K2Node_CompleteAdventure.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "AdventureAsset.h"

#define LOCTEXT_NAMESPACE "UAdventureGraphNode_Complete"

UK2Node_CompleteAdventure::UK2Node_CompleteAdventure()
{
	FunctionReference.SetFromField<UFunction>(UAdventureAsset::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureAsset, K2_CompleteAdventure)), true);
}

FLinearColor UK2Node_CompleteAdventure::GetNodeTitleColor() const
{
	return FColor::Green;
}

#undef LOCTEXT_NAMESPACE

// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGraphNodes/K2Node_FailAdventure.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "AdventureAsset.h"


#define LOCTEXT_NAMESPACE "AdventureGraphNode_Fail"

UK2Node_FailAdventure::UK2Node_FailAdventure()
{ 
	FunctionReference.SetFromField<UFunction>(UAdventureAsset::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureAsset, K2_FailAdventure)), true);
}
FLinearColor UK2Node_FailAdventure::GetNodeTitleColor() const
{
	return FColor::Orange;
}

#undef LOCTEXT_NAMESPACE

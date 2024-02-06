// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_AppendObjective.h"
#include "AdventureAsset.h"

const FName UK2Node_AppendObjective::NAME_ObjectiveArray(TEXT("ObjectiveArray"));
const FName UK2Node_AppendObjective::NAME_ObjectiveInfo(TEXT("ObjectiveEntry"));

UK2Node_AppendObjective::UK2Node_AppendObjective()
{
	FunctionReference.SetFromField<UFunction>(UAdventureAsset::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureAsset, MakeObjectiveArray)), true);
}

UEdGraphPin* UK2Node_AppendObjective::GetObjectiveArrayPin()
{
	return FindPin(NAME_ObjectiveArray);
}

UEdGraphPin* UK2Node_AppendObjective::GetObjectivePin()
{
	return FindPin(NAME_ObjectiveInfo);
}

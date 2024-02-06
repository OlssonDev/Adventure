// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_CallFunction.h"
#include "K2Node_AppendObjective.generated.h"


UCLASS()
class UK2Node_AppendObjective : public UK2Node_CallFunction
{
	GENERATED_BODY()

	public:

	UK2Node_AppendObjective();

	UEdGraphPin* GetObjectiveArrayPin();
	UEdGraphPin* GetObjectivePin();

	private:
	
	static const FName NAME_ObjectiveArray;
	static const FName NAME_ObjectiveInfo;

};

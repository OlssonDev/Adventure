// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_CallFunction.h"
#include "K2Node_BreakObjective.generated.h"

UCLASS()
class ADVENTUREGRAPHEDITOR_API UK2Node_BreakObjective : public UK2Node_CallFunction
{
	GENERATED_BODY()

	public:

	UK2Node_BreakObjective();

	//UEdGraphNode interface implementation
	virtual void PostPlacedNewNode() override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//End of implementation
};

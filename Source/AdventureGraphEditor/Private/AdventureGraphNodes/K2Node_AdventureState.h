// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_CallFunction.h"
#include "K2Node_AdventureState.generated.h"

UCLASS()
class ADVENTUREGRAPHEDITOR_API UK2Node_AdventureState : public UK2Node_CallFunction
{
	GENERATED_BODY()

	//Start of UEdGraphNode implementation
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual FText GetMenuCategory() const override;
	virtual bool CanCreateUnderSpecifiedSchema(const UEdGraphSchema* Schema) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//End of implementation
};

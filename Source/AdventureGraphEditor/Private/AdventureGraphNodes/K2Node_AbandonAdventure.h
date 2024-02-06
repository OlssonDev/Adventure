// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_AdventureState.h"
#include "K2Node_AbandonAdventure.generated.h"

UCLASS()
class ADVENTUREGRAPHEDITOR_API UK2Node_AbandonAdventure : public UK2Node_AdventureState
{
	GENERATED_BODY()

	public:

	UK2Node_AbandonAdventure();

	//Start of UEdGraphNode interface implementation
	virtual FLinearColor GetNodeTitleColor() const override;
	//End of implementation
};

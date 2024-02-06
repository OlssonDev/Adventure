// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_Event.h"
#include "K2Node_Root.generated.h"

/**
 * 
 */
UCLASS()
class ADVENTUREGRAPHEDITOR_API UK2Node_Root : public UK2Node_Event
{
	GENERATED_BODY()
	
	public:
	
	UK2Node_Root();
	
	//Begin UEdGraphNode implementation
	virtual bool CanUserDeleteNode() const override
	{
		return false;
	};
	virtual bool CanDuplicateNode() const override
	{
		return false;
	};
	virtual bool IsNodeRootSet() const override
	{
		return true;
	};

	virtual void AllocateDefaultPins() override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override {};

	virtual void PostPlacedNewNode() override;
	virtual FLinearColor GetNodeTitleColor() const override;;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	//End of implementation
};

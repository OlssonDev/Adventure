// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_CallFunction.h"
#include "K2Node_MakeObjective.generated.h"

//Creates a objective with a localized description.
UCLASS()
class ADVENTUREGRAPHEDITOR_API UK2Node_MakeObjective : public UK2Node_CallFunction
{
	GENERATED_BODY()

	public:
	
	UK2Node_MakeObjective();
	
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool IsNodePure() const override;
	virtual void AllocateDefaultPins() override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual void PostPlacedNewNode() override;
	
	
};
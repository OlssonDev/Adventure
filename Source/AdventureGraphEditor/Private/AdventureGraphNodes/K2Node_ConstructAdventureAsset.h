// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_ConstructObjectFromClass.h"
#include "K2Node_ConstructAdventureAsset.generated.h"

UCLASS()
class ADVENTUREGRAPHEDITOR_API UK2Node_ConstructAdventureAsset : public UK2Node_ConstructObjectFromClass
{
	GENERATED_BODY()

	public:
	
	//Start of UK2Node_ConstructObjectFromClass interface implementation
	virtual UClass* GetClassPinBaseClass() const override;
	virtual bool IsSpawnVarPin(UEdGraphPin* Pin) const override;
	//End of implementation
	
	//UEdGraphNode interface implementation
	virtual FText GetBaseNodeTitle() const override;
	virtual void AllocateDefaultPins() override;
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual FText GetNodeTitleFormat() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetMenuCategory() const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	//End of implementation

	//Start of UK2Node implementation
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	//End of implementation
};

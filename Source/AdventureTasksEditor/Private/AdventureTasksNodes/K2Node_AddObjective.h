// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_AddPinInterface.h"
#include "K2Node_BaseAsyncTask.h"
#include "K2Node_LatentAdventureTaskCall.h"
#include "K2Node_AddObjective.generated.h"

UCLASS()
class UK2Node_AddObjective : public UK2Node_LatentAdventureTaskCall, public IK2Node_AddPinInterface
{
	GENERATED_BODY()

	public:

	UK2Node_AddObjective(const FObjectInitializer& ObjectInitializer);

	//UK2Node_LatentAdventureTaskCall interface implementation
	virtual bool IsHandling(TSubclassOf<UAdventureTask> TaskClass) const override;
	//End of implementation

	//UK2Node interface implementation
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	virtual bool IsCompatibleWithGraph(UEdGraph const* Graph) const override;
	virtual void AllocateDefaultPins() override;
	virtual void GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
	virtual void RemoveInputPin(UEdGraphPin* Pin) override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	//End of implementation

	//UK2Node_AddPinInterface implementation
	virtual void AddInputPin() override;
	virtual bool CanAddPin() const override;
	//End of implementation

	void AddInputPin_Internal(int32 Index);
	void ChangeInputMode();
	bool CanRemoveInputPin(const UEdGraphPin* PinToRemove) const;
	FName GetPinName(int32 Index);
	void CreateObjectivesArray();

	void SetUserCreatedPinsVisibility(bool IsVisible);
	void SetArrayInputVisibility(bool bVisible);
	void ResetToDefaultInput();
	
	public:

	UPROPERTY()
	bool bUseArray = false;

	/** The number of additional input pins to generate for this node (2 base pins are not included) */
	UPROPERTY()
	int32 NumAdditionalInputs;

	UPROPERTY()
	TArray<FName> UserCreatedObjectivePins;

	private:

	static const FName NAME_ObjectivesArray;
};
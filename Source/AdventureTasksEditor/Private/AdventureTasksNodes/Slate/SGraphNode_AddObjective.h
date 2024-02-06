// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KismetNodes/SGraphNodeK2Base.h"

class UK2Node;

class SGraphNode_AddObjective : public SGraphNodeK2Base
{
	public:
	SLATE_BEGIN_ARGS(SGraphNode_AddObjective){}
	SLATE_END_ARGS()

	void Construct( const FArguments& InArgs, UK2Node* InNode );

	FText GetInputModeButtonText();

	//Gets the construct adventure objective node
	class UK2Node_AddObjective* GetOwningNode();

	protected:
	
	// SGraphNode interface
	virtual void CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox) override;
	virtual FReply OnAddPin() override;
	virtual EVisibility IsAddPinButtonVisible() const override;
	// End of SGraphNode interface
	
};

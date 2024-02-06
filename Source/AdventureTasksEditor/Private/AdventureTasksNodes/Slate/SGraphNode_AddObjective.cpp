// Fill out your copyright notice in the Description page of Project Settings.


#include "SGraphNode_AddObjective.h"
#include "GraphEditorSettings.h"
#include "K2Node.h"
#include "K2Node_AddPinInterface.h"
#include "ScopedTransaction.h"
#include "AdventureTasksNodes/K2Node_AddObjective.h"

#define LOCTEXT_NAMESPACE "SGraphNode_CreateObjective"

void SGraphNode_AddObjective::Construct(const FArguments& InArgs, UK2Node* InNode)
{
	ensure(InNode == nullptr || InNode->GetClass()->ImplementsInterface(UK2Node_AddPinInterface::StaticClass()));
	GraphNode = InNode;
	SetCursor( EMouseCursor::CardinalCross );
	UpdateGraphNode();
}

FText SGraphNode_AddObjective::GetInputModeButtonText()
{
	if (GetOwningNode()->bUseArray)
	{
		return LOCTEXT("SGraphNode_CreateObjective_RestoreDefault", "Restore To Default Input");
	}
	return LOCTEXT("SGraphNode_CreateObjective_SwitchArray", "Convert To Array Input");
}

UK2Node_AddObjective* SGraphNode_AddObjective::GetOwningNode()
{
	return CastChecked<UK2Node_AddObjective>(GraphNode);
}

void SGraphNode_AddObjective::CreateOutputSideAddButton(TSharedPtr<SVerticalBox> OutputBox)
{
	TSharedRef<SWidget> AddPinButton = AddPinButtonContent(
		NSLOCTEXT("SequencerNode", "SGraphNode_CreateObjective_AddPinButtonText", "Add Objective Pin"),
		NSLOCTEXT("SequencerNode", "SGraphNode_CreateObjective_AddPinButtonToolTip", "Add new objective"));

	FMargin AddPinPadding = Settings->GetOutputPinPadding();
	AddPinPadding.Top += 6.0f;

	OutputBox->AddSlot()
	.AutoHeight()
	.VAlign(VAlign_Center)
	.HAlign(HAlign_Right)
	.Padding(AddPinPadding)
	[
		AddPinButton
	];

	AddPinButton->SetVisibility(GetOwningNode()->bUseArray ? EVisibility::Collapsed : EVisibility::Visible);
}

FReply SGraphNode_AddObjective::OnAddPin()
{
	IK2Node_AddPinInterface* AddPinNode = Cast<IK2Node_AddPinInterface>(GraphNode);
	ensure(AddPinNode);
	if (AddPinNode && AddPinNode->CanAddPin())
	{
		FScopedTransaction Transaction(NSLOCTEXT("SequencerNode", "AddPinTransaction", "Add Pin"));

		AddPinNode->AddInputPin();
		UpdateGraphNode();
		GraphNode->GetGraph()->NotifyGraphChanged();
	}
	
	return FReply::Handled();
}

EVisibility SGraphNode_AddObjective::IsAddPinButtonVisible() const
{
	return EVisibility::Visible;
}

#undef LOCTEXT_NAMESPACE

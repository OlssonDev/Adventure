// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGraphSchema.h"

#include "AdventureEdGraph.h"
#include "EdGraphSchema_K2_Actions.h"
#include "ToolMenu.h"
#include "AdventureGraphNodes/K2Node_Root.h"
#include "AdventureAsset.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "AdventureSchema"

FName UAdventureGraphSchema::GetParentContextMenuName() const
{
	return "Adventure Graph";
}

void UAdventureGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	if (Cast<UAdventureEdGraph>(&Graph))
	{
		FGraphNodeCreator<UK2Node_Root> RootNodeCreator(Graph);
		UK2Node_Root* RootNode = RootNodeCreator.CreateNode();
		RootNode->SetMakeCommentBubbleVisible(true);
		RootNode->OnUpdateCommentText("Drag out pin to build functionality for the Adventure.");
		RootNodeCreator.Finalize();
		SetNodeMetaData(RootNode, FNodeMetadata::DefaultGraphNode);

		FGraphNodeCreator<UK2Node_Event> AdventureCompletedNodeCreator(Graph);
		UK2Node_Event* AdventureCompletedNode = AdventureCompletedNodeCreator.CreateNode();
		AdventureCompletedNode->NodePosY += 150.f;
		AdventureCompletedNode->EventReference.SetFromField<UFunction>(UAdventureAsset::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureAsset, K2_OnAdventureCompleted)), true);
		AdventureCompletedNode->SetMakeCommentBubbleVisible(true);
		AdventureCompletedNode->MakeAutomaticallyPlacedGhostNode();
		AdventureCompletedNode->bOverrideFunction = true;
		AdventureCompletedNodeCreator.Finalize();

		FGraphNodeCreator<UK2Node_Event> AdventureFailedNodeCreator(Graph);
		UK2Node_Event* AdventureFailedNode = AdventureFailedNodeCreator.CreateNode();
		AdventureFailedNode->NodePosY += 300.f;
		AdventureFailedNode->EventReference.SetFromField<UFunction>(UAdventureAsset::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureAsset, K2_OnAdventureFailed)), true);
		AdventureFailedNode->SetMakeCommentBubbleVisible(true);
		AdventureFailedNode->MakeAutomaticallyPlacedGhostNode();
		AdventureFailedNode->bOverrideFunction = true;
		AdventureFailedNodeCreator.Finalize();

		FGraphNodeCreator<UK2Node_Event> NewAdventureStateNodeCreator(Graph);
		UK2Node_Event* NewAdventureStateNode = NewAdventureStateNodeCreator.CreateNode();
		NewAdventureStateNode->NodePosY += 450.f;
		NewAdventureStateNode->EventReference.SetFromField<UFunction>(UAdventureAsset::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureAsset, K2_OnNewAdventureState)), true);
		NewAdventureStateNode->SetMakeCommentBubbleVisible(true);
		NewAdventureStateNode->MakeAutomaticallyPlacedGhostNode();
		NewAdventureStateNode->bOverrideFunction = true;
		NewAdventureStateNodeCreator.Finalize();

		FGraphNodeCreator<UK2Node_Event> NewAdventureInitNodeCreator(Graph);
		UK2Node_Event* NewAdventureInitNode = NewAdventureInitNodeCreator.CreateNode();
		NewAdventureInitNode->NodePosY += 600.f;
		NewAdventureInitNode->EventReference.SetFromField<UFunction>(UAdventureAsset::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureAsset, K2_OnAdventureInitialized)), true);
		NewAdventureInitNode->SetMakeCommentBubbleVisible(true);
		NewAdventureInitNode->MakeAutomaticallyPlacedGhostNode();
		NewAdventureInitNode->bOverrideFunction = true;
		NewAdventureInitNodeCreator.Finalize();

		SetNodeMetaData(RootNode, FNodeMetadata::DefaultGraphNode);
	}
	else
	{
		Super::CreateDefaultNodesForGraph(Graph);
	}
}

void UAdventureGraphSchema::GetGraphDisplayInformation(const UEdGraph& Graph, FGraphDisplayInfo& DisplayInfo) const
{
	if (Cast<UAdventureEdGraph>(&Graph))
	{
		DisplayInfo.PlainName = FText::FromString(Graph.GetName());
		DisplayInfo.DisplayName = DisplayInfo.PlainName;
		DisplayInfo.Tooltip = LOCTEXT("GraphTooltip_AdventureGraph", "Graph used to plan and create your Adventure");
	}
	else
	{
		Super::GetGraphDisplayInformation(Graph, DisplayInfo);
	}
}

void UAdventureGraphSchema::GetPaletteActions(FGraphActionMenuBuilder& ActionMenuBuilder)
{
	//Will implement
}

#if ENGINE_MAJOR_VERSION ==  5
FText UAdventureGraphSchema::GetGraphCategory(const UEdGraph* InGraph) const
{
	return FText::FromString("Graphs");
}
#endif

#if ENGINE_MAJOR_VERSION ==  5
FReply UAdventureGraphSchema::TrySetGraphCategory(const UEdGraph* InGraph, const FText& InCategory)
{
	return FReply::Handled();
}
#endif

#undef LOCTEXT_NAMESPACE

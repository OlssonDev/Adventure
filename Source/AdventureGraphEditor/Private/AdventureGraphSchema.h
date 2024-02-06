// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraphSchema_K2.h"
#include "AdventureGraphSchema.generated.h"

UCLASS()
class UAdventureGraphSchema : public UEdGraphSchema_K2
{
	GENERATED_BODY()

	public:

	//UEDGraphSchema Interface Implementation
	virtual FName GetParentContextMenuName() const override;
	virtual void CreateDefaultNodesForGraph(UEdGraph& Graph) const override;
	virtual void GetGraphDisplayInformation(const UEdGraph& Graph, FGraphDisplayInfo& DisplayInfo) const override;

	#if ENGINE_MAJOR_VERSION == 5
	virtual FReply TrySetGraphCategory(const UEdGraph* InGraph, const FText& InCategory) override;
	virtual FText GetGraphCategory(const UEdGraph* InGraph) const override;
	#endif
	//End of implementation

	void GetPaletteActions(FGraphActionMenuBuilder& ActionMenuBuilder);
};

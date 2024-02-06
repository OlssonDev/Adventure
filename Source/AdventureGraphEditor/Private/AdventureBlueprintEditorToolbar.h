// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMGEditor/Private/WidgetBlueprintEditorToolbar.h"

class IAdventureGraphEditor;
class FExtender;
class FToolBarBuilder;
class UToolMenu;

class FAdventureBlueprintEditorToolbar : public TSharedFromThis<FAdventureBlueprintEditorToolbar>
{

	public:
	
	FAdventureBlueprintEditorToolbar(TSharedPtr<IAdventureGraphEditor>& InWidgetEditor);
	
	void AddAdventureBlueprintEditorModesToolbar(TSharedPtr<FExtender> Extender);
	void AddAdventureGraphAssetCreatorButtons(TSharedPtr<FExtender> Extender);
	void FillWidgetBlueprintEditorModesToolbar(FToolBarBuilder& ToolbarBuilder);
	void FillWidgetAssetCreatorButtons(FToolBarBuilder& ToolbarBuilder);
	
	void AddAdventureButtons(UToolMenu* InMenu) const;
	
	TWeakPtr<IAdventureGraphEditor> WidgetEditor;
};

struct FAdventureBlueprintApplicationModes
{
	static const FName AdventureMode;
	static const FName GraphMode;

	static FText GetLocalizedMode(const FName InMode)
	{
		static TMap< FName, FText > LocModes;

		if ( LocModes.Num() == 0 )
		{
			LocModes.Add(AdventureMode, NSLOCTEXT("AdventureBlueprintModes", "Adventure", "Adventure"));
			LocModes.Add(GraphMode, NSLOCTEXT("AdventureBlueprintModes", "GraphMode", "Graph"));
		}

		check(InMode != NAME_None);
		const FText* OutDesc = LocModes.Find(InMode);
		check(OutDesc);

		return *OutDesc;
	}

private:
	
	FAdventureBlueprintApplicationModes() {}
};

const FName FAdventureBlueprintApplicationModes::AdventureMode("AdventureEditorMode");
const FName FAdventureBlueprintApplicationModes::GraphMode("GraphName");

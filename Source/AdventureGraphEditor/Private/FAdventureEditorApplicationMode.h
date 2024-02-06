// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintEditorModes.h"

class FAdventureBlueprintApplicationMode : public FBlueprintEditorApplicationMode
{
public:
	
	FAdventureBlueprintApplicationMode(TSharedPtr<class IAdventureGraphEditor> InAdventureEditor, FName InModeName);

	protected:
	
	class UAdventureAssetBlueprint* GetBlueprint() const;
	TSharedPtr<class IAdventureGraphEditor> GetBlueprintEditor() const;

	//FBlueprintEditorApplicationMode implementation
	virtual void RegisterToolbarMenus(UToolMenu* Toolbar, TSharedPtr<class IAdventureGraphEditor> InAdventureEditor);
	virtual void RegisterTabFactories(TSharedPtr<FTabManager> InTabManager) override;
	virtual void PostActivateMode() override;
	//End of implementation

	protected:
	
	TWeakPtr<class IAdventureGraphEditor> AdventureBlueprintEditor;
	FWorkflowAllowedTabSet TabFactories;
};

class FAdventureEditorApplicationMode : public FAdventureBlueprintApplicationMode
{
	
	public:

	FAdventureEditorApplicationMode(TSharedPtr<class IAdventureGraphEditor> InAdventureEditor);

	//FBlueprintEditorApplicationMode implementation
	virtual void PostActivateMode() override;
	virtual void RegisterToolbarMenus(UToolMenu* Toolbar, TSharedPtr<IAdventureGraphEditor> InAdventureEditor) override;
	//End of implementation
};

class FAdventureBlueprintGraphApplicationMode : public FAdventureBlueprintApplicationMode
{
	
	public:

	FAdventureBlueprintGraphApplicationMode(TSharedPtr<class IAdventureGraphEditor> InAdventureEditor);
	
	//FBlueprintEditorApplicationMode implementation
	virtual void PostActivateMode() override;
	virtual void RegisterToolbarMenus(UToolMenu* Toolbar, TSharedPtr<IAdventureGraphEditor> InAdventureEditor) override;
	//End of implementation
};



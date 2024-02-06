// Fill out your copyright notice in the Description page of Project Settings.


#include "FAdventureEditorApplicationMode.h"
#include "AdventureBlueprintEditorToolbar.h"
#include "AdventureGraphEditor.h"
#include "AdventureGraphEditorModule.h"
#include "SBlueprintEditorToolbar.h"
#include "ToolMenu.h"

FAdventureEditorApplicationMode::FAdventureEditorApplicationMode(TSharedPtr<class IAdventureGraphEditor> InAdventureEditor) : FAdventureBlueprintApplicationMode(InAdventureEditor, FAdventureBlueprintApplicationModes::AdventureMode)
{
}

void FAdventureBlueprintApplicationMode::PostActivateMode()
{
	FBlueprintEditorApplicationMode::PostActivateMode();
	
	if (UToolMenu* Toolbar = AdventureBlueprintEditor.Pin()->RegisterModeToolbarIfUnregistered(GetModeName()))
	{
		RegisterToolbarMenus(Toolbar, GetBlueprintEditor());
	}
	;
	
	IAdventureGraphEditorModule& AdventureGraphEditorModule = FModuleManager::LoadModuleChecked<IAdventureGraphEditorModule>("AdventureGraphEditor");
	ToolbarExtender = AdventureGraphEditorModule.GetToolBarExtensibilityManager()->GetAllExtenders();
	
	// AdventureBlueprintEditor.Pin()->AdventureToolbar->AddAdventureBlueprintEditorModesToolbar(ToolbarExtender);
}

void FAdventureEditorApplicationMode::PostActivateMode()
{
	FAdventureBlueprintApplicationMode::PostActivateMode();
	
	GetBlueprintEditor()->CreateAdventureGraph();
	AdventureBlueprintEditor.Pin()->AdventureToolbar->AddAdventureGraphAssetCreatorButtons(ToolbarExtender);
}

void FAdventureEditorApplicationMode::RegisterToolbarMenus(UToolMenu* Toolbar, TSharedPtr<IAdventureGraphEditor> InAdventureEditor)
{
	FAdventureBlueprintApplicationMode::RegisterToolbarMenus(Toolbar, InAdventureEditor);
	// InAdventureEditor->AdventureToolbar->AddAdventureButtons(Toolbar);
}

FAdventureBlueprintGraphApplicationMode::FAdventureBlueprintGraphApplicationMode(TSharedPtr<IAdventureGraphEditor> InAdventureEditor) : FAdventureBlueprintApplicationMode(InAdventureEditor, FAdventureBlueprintApplicationModes::GraphMode)
{
	
}

void FAdventureBlueprintGraphApplicationMode::PostActivateMode()
{
	FAdventureBlueprintApplicationMode::PostActivateMode();
	GetBlueprintEditor()->InitEventGraph();
}

void FAdventureBlueprintGraphApplicationMode::RegisterToolbarMenus(UToolMenu* Toolbar, TSharedPtr<IAdventureGraphEditor> InAdventureEditor)
{
	FAdventureBlueprintApplicationMode::RegisterToolbarMenus(Toolbar, InAdventureEditor);
}


FAdventureBlueprintApplicationMode::FAdventureBlueprintApplicationMode(TSharedPtr<IAdventureGraphEditor> InAdventureEditor, FName InModeName) : FBlueprintEditorApplicationMode(InAdventureEditor, InModeName, FAdventureBlueprintApplicationModes::GetLocalizedMode, false, false), AdventureBlueprintEditor(InAdventureEditor)
{
	
}

UAdventureAssetBlueprint* FAdventureBlueprintApplicationMode::GetBlueprint() const
{
	if (const IAdventureGraphEditor* Editor = AdventureBlueprintEditor.Pin().Get())
	{
		return Editor->GetAdventureBlueprintObj();
	}

	return nullptr;
}

TSharedPtr<IAdventureGraphEditor> FAdventureBlueprintApplicationMode::GetBlueprintEditor() const
{
	return AdventureBlueprintEditor.Pin();
}

void FAdventureBlueprintApplicationMode::RegisterToolbarMenus(UToolMenu* Toolbar, TSharedPtr<IAdventureGraphEditor> InAdventureEditor)
{
	InAdventureEditor->GetToolbarBuilder()->AddCompileToolbar(Toolbar);
	InAdventureEditor->GetToolbarBuilder()->AddScriptingToolbar(Toolbar);
	InAdventureEditor->GetToolbarBuilder()->AddBlueprintGlobalOptionsToolbar(Toolbar);
	InAdventureEditor->GetToolbarBuilder()->AddDebuggingToolbar(Toolbar);
}

void FAdventureBlueprintApplicationMode::RegisterTabFactories(TSharedPtr<FTabManager> InTabManager)
{
	FBlueprintEditorApplicationMode::RegisterTabFactories(InTabManager);
	GetBlueprintEditor()->PushTabFactories(TabFactories);
}

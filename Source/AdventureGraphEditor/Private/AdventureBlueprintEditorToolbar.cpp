// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureBlueprintEditorToolbar.h"

#include "AdventureGraphEditor.h"
#include "BlueprintEditorContext.h"
#include "EditorStyleSet.h"
#include "FAdventureEditorCommands.h"
#include "IDocumentation.h"
#include "SBlueprintEditorToolbar.h"
#include "ToolMenu.h"
#include "ToolMenuMisc.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Widgets/Layout/SSpacer.h"
#include "WorkflowOrientedApp/SModeWidget.h"

#define LOCTEXT_NAMESPACE "Adventure"

FAdventureBlueprintEditorToolbar::FAdventureBlueprintEditorToolbar(TSharedPtr<IAdventureGraphEditor>& InWidgetEditor)
	: WidgetEditor(InWidgetEditor)
{
}

void FAdventureBlueprintEditorToolbar::AddAdventureBlueprintEditorModesToolbar(TSharedPtr<FExtender> Extender)
{
	Extender->AddToolBarExtension("Asset",EExtensionHook::After,WidgetEditor.Pin()->GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateSP(this, &FAdventureBlueprintEditorToolbar::FillWidgetBlueprintEditorModesToolbar));
}

void FAdventureBlueprintEditorToolbar::AddAdventureGraphAssetCreatorButtons(TSharedPtr<FExtender> Extender)
{
	Extender->AddToolBarExtension("Asset",EExtensionHook::After,WidgetEditor.Pin()->GetToolkitCommands(),
	FToolBarExtensionDelegate::CreateSP(this, &FAdventureBlueprintEditorToolbar::FillWidgetAssetCreatorButtons));
}

void FAdventureBlueprintEditorToolbar::FillWidgetBlueprintEditorModesToolbar(FToolBarBuilder& ToolbarBuilder)
{
	TSharedPtr<IAdventureGraphEditor> BlueprintEditorPtr = WidgetEditor.Pin();
	UBlueprint* BlueprintObj = BlueprintEditorPtr->GetBlueprintObj();

	if(!BlueprintObj || (!FBlueprintEditorUtils::IsLevelScriptBlueprint(BlueprintObj) && !FBlueprintEditorUtils::IsInterfaceBlueprint(BlueprintObj) && !BlueprintObj->bIsNewlyCreated))
	{
		const TAttribute<FName> GetActiveMode(BlueprintEditorPtr.ToSharedRef(), &FBlueprintEditor::GetCurrentMode);
		const FOnModeChangeRequested SetActiveMode = FOnModeChangeRequested::CreateSP(BlueprintEditorPtr.ToSharedRef(), &FBlueprintEditor::SetCurrentMode);

		// Left side padding
		BlueprintEditorPtr->AddToolbarWidget(SNew(SSpacer).Size(FVector2D(4.0f, 1.0f)));

		BlueprintEditorPtr->AddToolbarWidget(
			SNew(SModeWidget, FAdventureBlueprintApplicationModes::GetLocalizedMode(FAdventureBlueprintApplicationModes::AdventureMode), FAdventureBlueprintApplicationModes::AdventureMode)
			.OnGetActiveMode(GetActiveMode)
			.OnSetActiveMode(SetActiveMode)
			.ToolTip(IDocumentation::Get()->CreateToolTip(
				LOCTEXT("DesignerModeButtonTooltip", "Switch to Adventure Graph"),
				NULL,
				TEXT("Shared/Editors/BlueprintEditor"),
				TEXT("AdventureMode")))
			.IconImage(FEditorStyle::GetBrush("UMGEditor.SwitchToDesigner"))
			.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("DesignerMode")))
		);

		BlueprintEditorPtr->AddToolbarWidget(SNew(SSpacer).Size(FVector2D(10.0f, 1.0f)));

		BlueprintEditorPtr->AddToolbarWidget(
			SNew(SModeWidget, FAdventureBlueprintApplicationModes::GetLocalizedMode(FAdventureBlueprintApplicationModes::GraphMode), FAdventureBlueprintApplicationModes::GraphMode)
			.OnGetActiveMode(GetActiveMode)
			.OnSetActiveMode(SetActiveMode)
			.CanBeSelected(BlueprintEditorPtr.Get(), &FBlueprintEditor::IsEditingSingleBlueprint)
			.ToolTip(IDocumentation::Get()->CreateToolTip(
				LOCTEXT("GraphModeButtonTooltip", "Switch to Graph Editing Mode"),
				NULL,
				TEXT("Shared/Editors/BlueprintEditor"),
				TEXT("GraphMode")))
			.ToolTipText(LOCTEXT("GraphModeButtonTooltip", "Switch to Graph Editing Mode"))
			.IconImage(FEditorStyle::GetBrush("FullBlueprintEditor.SwitchToScriptingMode"))
			.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("GraphMode")))
		);

		
		BlueprintEditorPtr->AddToolbarWidget(SNew(SSpacer).Size(FVector2D(10.0f, 1.0f)));
	}
}

void FAdventureBlueprintEditorToolbar::FillWidgetAssetCreatorButtons(FToolBarBuilder& ToolbarBuilder)
{
	TSharedPtr<IAdventureGraphEditor> BlueprintEditorPtr = WidgetEditor.Pin();
	UBlueprint* BlueprintObj = BlueprintEditorPtr->GetBlueprintObj();
		
	ToolbarBuilder.EndSection();
}


void FAdventureBlueprintEditorToolbar::AddAdventureButtons(UToolMenu* InMenu) const
{
	FToolMenuSection& Section = InMenu->AddSection("AdventureSection");
	Section.InsertPosition = FToolMenuInsert("Asset", EToolMenuInsertType::Before);

	Section.AddDynamicEntry("Adventure Settings", FNewToolMenuSectionDelegate::CreateLambda([](FToolMenuSection& InSection)
	{
		const UBlueprintEditorToolMenuContext* Context = InSection.FindContext<UBlueprintEditorToolMenuContext>();
		if (Context && Context->BlueprintEditor.IsValid() && Context->GetBlueprintObj())
		{
			const TSharedPtr<class FBlueprintEditorToolbar> BlueprintEditorToolbar = Context->BlueprintEditor.Pin()->GetToolbarBuilder();
			if (BlueprintEditorToolbar.IsValid())
			{
				InSection.AddEntry(FToolMenuEntry::InitToolBarButton(
				FAdventureEditorCommands::Get().AdventureSettings,
				TAttribute<FText>(),
				TAttribute<FText>(),
				FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.GameSettings")
			));
			}
		}
	}));
}

#undef LOCTEXT_NAMESPACE

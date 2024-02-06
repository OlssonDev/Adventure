// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureEditorTabFactories.h"
#include "AdventureGraphEditor.h"
#include "EditorStyleSet.h"
#include "BlueprintAssets/AdventureAssetBlueprint.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "AdventureEditorTabFactories"

FAdventureEditorTabFactory::FAdventureEditorTabFactory(TSharedPtr<IAdventureGraphEditor> InAdventureEditor) : FDocumentTabFactoryForObjects<UEdGraph>("AdventureEditor_Properties", InAdventureEditor), AdventureEditor(InAdventureEditor)
{
	
}

void FAdventureEditorTabFactory::OnTabActivated(TSharedPtr<SDockTab> Tab) const
{
	check(AdventureEditor.IsValid());	
	TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	AdventureEditor.Pin()->OnGraphEditorFocused(GraphEditor);
}

void FAdventureEditorTabFactory::OnTabRefreshed(TSharedPtr<SDockTab> Tab) const
{
	TSharedRef<SGraphEditor> GraphEditor = StaticCastSharedRef<SGraphEditor>(Tab->GetContent());
	GraphEditor->NotifyGraphChanged();
}

TAttribute<FText> FAdventureEditorTabFactory::ConstructTabNameForObject(UEdGraph* DocumentID) const
{
	return TAttribute<FText>(FText::FromString(DocumentID->GetName()));
}

TSharedRef<SWidget> FAdventureEditorTabFactory::CreateTabBodyForObject(const FWorkflowTabSpawnInfo& Info, UEdGraph* DocumentID) const
{
	return OnCreateGraphEditorWidget.Execute(DocumentID);
}

const FSlateBrush* FAdventureEditorTabFactory::GetTabIconForObject(const FWorkflowTabSpawnInfo& Info, UEdGraph* DocumentID) const
{
	return FEditorStyle::GetBrush(TEXT("UMGEditor.SwitchToDesigner"));
}

void FAdventureEditorTabFactory::SaveState(TSharedPtr<SDockTab> Tab, TSharedPtr<FTabPayload> Payload) const
{
	FVector2D ViewLocation;
	float ZoomAmount;
	StaticCastSharedRef<SGraphEditor>(Tab->GetContent())->GetViewLocation(ViewLocation, ZoomAmount);
	AdventureEditor.Pin()->AdventureBlueprint->LastEditedDocuments.Add(FEditedDocumentInfo(FTabPayload_UObject::CastChecked<UEdGraph>(Payload), ViewLocation, ZoomAmount));
}

#undef LOCTEXT_NAMESPACE

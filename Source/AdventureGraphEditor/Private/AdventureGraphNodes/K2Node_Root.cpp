// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGraphNodes/K2Node_Root.h"
#include "KismetCompiler.h"
#include "AdventureAsset.h"

#define LOCTEXT_NAMESPACE "AdventureGraphNode_Root"

UK2Node_Root::UK2Node_Root()
{
	bOverrideFunction = true;
	EventReference.SetFromField<UFunction>(UAdventureAsset::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureAsset, K2_OnAdventureBeginPlay)), true);
}

FLinearColor UK2Node_Root::GetNodeTitleColor() const
{
	return FLinearColor::Red;
}

FSlateIcon UK2Node_Root::GetIconAndTint(FLinearColor& OutColor) const
{
	static const FSlateIcon Icon = FSlateIcon("EditorStyle", "ClassIcon.K2Node_Event");
	return Icon;
}

void UK2Node_Root::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	FindPin(DelegateOutputName)->SafeSetHidden(true);
}

void UK2Node_Root::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
}

#undef LOCTEXT_NAMESPACE

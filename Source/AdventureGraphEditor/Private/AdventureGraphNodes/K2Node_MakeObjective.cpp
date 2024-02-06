// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGraphNodes/K2Node_MakeObjective.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "KismetCompiler.h"
#include "ToolMenu.h"
#include "AdventurePluginSettings.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Library/AdventureBlueprintLibrary.h"

#define LOCTEXT_NAMESPACE "K2Node_MakeObjective"

static const FName PN_Argument = GET_MEMBER_NAME_CHECKED(FAdventureObjectiveEntry, Argument);
static const FName PN_Quantity = GET_MEMBER_NAME_CHECKED(FAdventureObjectiveEntry, Quantity);

UK2Node_MakeObjective::UK2Node_MakeObjective()
{
	FunctionReference.SetFromField<UFunction>(UAdventureBlueprintLibrary::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureBlueprintLibrary, MakeObjectiveWithName)), false);
}

void UK2Node_MakeObjective::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	const UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FSlateIcon UK2Node_MakeObjective::GetIconAndTint(FLinearColor& OutColor) const
{
	#if ENGINE_MAJOR_VERSION == 4
	static const FSlateIcon Icon = FSlateIcon("EditorStyle", "ClassIcon.StringTable");
	return Icon;
	#endif

	#if ENGINE_MAJOR_VERSION == 5
	static const FSlateIcon Icon = FSlateIcon("EditorStyle", "ClassIcon.StringTable");
	return Icon;
	#endif
}

FText UK2Node_MakeObjective::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("K2Node_MakeObjectiveTitle", "Make Objective");
}

bool UK2Node_MakeObjective::IsNodePure() const
{
	return true;
}

void UK2Node_MakeObjective::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	if (UEdGraphPin* QuantityPin = FindPin(PN_Quantity))
	{
		QuantityPin->DefaultValue = "1";
	}
	
	if (UEdGraphPin* ObjectiveArgumentPin = FindPin(PN_Argument))
	{
		if (UAdventurePluginSettings::IsUsingGameplayTags() == false)
		{
			ObjectiveArgumentPin->DefaultValue = "My Objective Argument";
		}

		ObjectiveArgumentPin->SetSavePinIfOrphaned(false);
	}
}

void UK2Node_MakeObjective::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) 
{
	if (UEdGraphPin* QuantityPin = FindPin(PN_Quantity))
	{
		if (QuantityPin->DefaultValue == "0" && QuantityPin->LinkedTo.Num() == 0)
		{
			CompilerContext.MessageLog.Warning(*LOCTEXT("MakeObjective_BadQuantity", "@@ is set to 0. This objective will not run correctly.").ToString(), QuantityPin);
		}
	}

	if (UEdGraphPin* ObjectiveArgumentPin = FindPin(PN_Argument))
	{
		if (ObjectiveArgumentPin->LinkedTo.Num() == 0)
		{
			if (ObjectiveArgumentPin->DefaultValue == "None")
			{
				CompilerContext.MessageLog.Error(*LOCTEXT("MakeObjective_InvalidObjectiveArgument_None", "@@ can't be set as None.").ToString(), ObjectiveArgumentPin);
				return;
			}

			if (ObjectiveArgumentPin->DefaultValue.IsEmpty())
			{
				CompilerContext.MessageLog.Error(*LOCTEXT("MakeObjective_InvalidObjectiveArgument_Empty", "@@ can't be empty.").ToString(), ObjectiveArgumentPin);
				return;
			}
		}
	}

	Super::ExpandNode(CompilerContext, SourceGraph);
}

void UK2Node_MakeObjective::PostPlacedNewNode()
{
	Super::PostPlacedNewNode();
	
	if (UAdventurePluginSettings::IsUsingGameplayTags())
	{
		FunctionReference.SetFromField<UFunction>(UAdventureBlueprintLibrary::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureBlueprintLibrary, MakeObjectiveWithTag)), false);
	}
	else
	{
		FunctionReference.SetFromField<UFunction>(UAdventureBlueprintLibrary::StaticClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UAdventureBlueprintLibrary, MakeObjectiveWithName)), false);
	}

	ReconstructNode();
}

#undef LOCTEXT_NAMESPACE

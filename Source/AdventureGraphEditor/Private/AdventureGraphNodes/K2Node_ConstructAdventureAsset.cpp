// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGraphNodes/K2Node_ConstructAdventureAsset.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "KismetCompiler.h"
#include "AdventureAsset.h"
#include "Library/AdventureBlueprintLibrary.h"

#define LOCTEXT_NAMESPACE "K2Node_ConstructAdventureAsset"

UClass* UK2Node_ConstructAdventureAsset::GetClassPinBaseClass() const
{
	return UAdventureAsset::StaticClass();
}

bool UK2Node_ConstructAdventureAsset::IsSpawnVarPin(UEdGraphPin* Pin) const
{
	return Super::IsSpawnVarPin(Pin) && Pin->PinName != "ActorToUse";
}

FText UK2Node_ConstructAdventureAsset::GetBaseNodeTitle() const
{
	return LOCTEXT("CreateAdventure_BaseTitle", "Start Adventure On Actor");
}

void UK2Node_ConstructAdventureAsset::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	UEdGraphPin* OwningPlayerPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, AActor::StaticClass(), "ActorToUse");
	SetPinToolTip(*OwningPlayerPin, LOCTEXT("OwningPlayerPinDescription", "The actor that will start this Adventure."));
}

void UK2Node_ConstructAdventureAsset::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	Super::ReallocatePinsDuringReconstruction(OldPins);
}

FText UK2Node_ConstructAdventureAsset::GetNodeTitleFormat() const
{
	return LOCTEXT("CreateAdventure_TitleFormat", "Start {ClassName} On Actor");
}

FText UK2Node_ConstructAdventureAsset::GetTooltipText() const
{
	return LOCTEXT("CreateAdventure_Tooltip", "Starts an Adventure on the given Actor, if it has a AdventureComponent attached. You can expose variables from the Adventure blueprint to this node for more dynamic quests.");
}

FText UK2Node_ConstructAdventureAsset::GetMenuCategory() const
{
	return LOCTEXT("K2Node_ConstructAdventureAsset_MenuCategory", "Adventure | Tasks");
}

FSlateIcon UK2Node_ConstructAdventureAsset::GetIconAndTint(FLinearColor& OutColor) const
{
	#if ENGINE_MAJOR_VERSION == 4
	static const FSlateIcon Icon = FSlateIcon("EditorStyle", "ClassIcon.StringTable");
	return Icon;
	#endif

	#if ENGINE_MAJOR_VERSION == 5
	static const FSlateIcon Icon = FSlateIcon("EditorStyle", "Starship/AssetIcons/StringTable_16");
	return Icon;
	#endif
}

void UK2Node_ConstructAdventureAsset::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	UClass* ActionKey = GetClass();
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

void UK2Node_ConstructAdventureAsset::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
	
	static const FName BeginSpawningBlueprintFuncName = GET_FUNCTION_NAME_CHECKED(UAdventureBlueprintLibrary, BeginSpawningAdventureOnActor);
	static const FName AdventureClassNameParam(TEXT("AdventureToStart"));
	static const FName ActorToUsePtrParam(TEXT("ActorToUse"));

	static const FName FinishSpawningFuncName = GET_FUNCTION_NAME_CHECKED(UAdventureBlueprintLibrary, FinishSpawningAdventureOnActor);
	static const FName AdventurePtrParam(TEXT("AdventureToStart"));

	UK2Node_ConstructAdventureAsset* SpawnNode = this;
	UEdGraphPin* SpawnNodeExec = SpawnNode->GetExecPin();
	UEdGraphPin* SpawnActorToUsePin = SpawnNode->FindPin(ActorToUsePtrParam);
	UEdGraphPin* SpawnClassPin = SpawnNode->GetClassPin();
	UEdGraphPin* SpawnNodeThen = SpawnNode->GetThenPin();
	UEdGraphPin* SpawnNodeResult = SpawnNode->GetResultPin();

	// Cache the class to spawn. Note, this is the compile time class that the pin was set to or the variable type it was connected to. Runtime it could be a child.
	UClass* ClassToSpawn = GetClassToSpawn();

	UClass* SpawnClass = (SpawnClassPin != NULL) ? Cast<UClass>(SpawnClassPin->DefaultObject) : NULL;
	if (!SpawnClassPin || ((0 == SpawnClassPin->LinkedTo.Num()) && (NULL == SpawnClass)))
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("SpawnAdventureNodeMissingClass_NoClassError", "@@ must have a valid Adventure assigned to @@.").ToString(), SpawnNode, SpawnClassPin);
		SpawnNode->BreakAllNodeLinks();
		return;
	}
	
	if (SpawnClass == UAdventureAsset::StaticClass())
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("SpawnActorNodeMissingClass_BaseClassError", "@@ can't use the Adventure Base Class to start an Adventure. Assign a child class of Adventure to @@ instead.").ToString(), SpawnNode, SpawnClassPin);
		SpawnNode->BreakAllNodeLinks();
		return;
	}

	if (SpawnActorToUsePin->LinkedTo.Num() == false)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("SpawnActorNodeMissingClass_NoSpawnActor", "@@ must have a valid reference assigned to @@").ToString(), SpawnNode, SpawnActorToUsePin);
		SpawnNode->BreakAllNodeLinks();
		return;
	}
	
	UK2Node_CallFunction* CallBeginSpawnNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(SpawnNode, SourceGraph);
	CallBeginSpawnNode->FunctionReference.SetExternalMember(BeginSpawningBlueprintFuncName, UAdventureBlueprintLibrary::StaticClass());
	CallBeginSpawnNode->AllocateDefaultPins();

	UEdGraphPin* CallBeginExec = CallBeginSpawnNode->GetExecPin();
	UEdGraphPin* CallBeginActorToUsePtr = CallBeginSpawnNode->FindPin(ActorToUsePtrParam);
	UEdGraphPin* CallBeginActorClassPin = CallBeginSpawnNode->FindPin(AdventureClassNameParam);
	UEdGraphPin* CallBeginResult = CallBeginSpawnNode->GetReturnValuePin();	

	// Move 'exec' connection from spawn node to 'begin spawn'
	CompilerContext.MovePinLinksToIntermediate(*SpawnNodeExec, *CallBeginExec);

	if(SpawnClassPin->LinkedTo.Num() > 0)
	{
		// Copy the 'blueprint' connection from the spawn node to 'begin spawn'
		CompilerContext.MovePinLinksToIntermediate(*SpawnClassPin, *CallBeginActorClassPin);
	}
	else
	{
		// Copy blueprint literal onto begin spawn call 
		CallBeginActorClassPin->DefaultObject = SpawnClass;
	}

	// Copy the ActorToUse connection from the spawn node to 'begin spawn'
	if (SpawnActorToUsePin)
	{
		CompilerContext.MovePinLinksToIntermediate(*SpawnActorToUsePin, *CallBeginActorToUsePtr);
	}
	
	UK2Node_CallFunction* CallFinishSpawnNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(SpawnNode, SourceGraph);
	CallFinishSpawnNode->FunctionReference.SetExternalMember(FinishSpawningFuncName, UAdventureBlueprintLibrary::StaticClass());
	CallFinishSpawnNode->AllocateDefaultPins();

	UEdGraphPin* CallFinishExec = CallFinishSpawnNode->GetExecPin();
	UEdGraphPin* CallFinishThen = CallFinishSpawnNode->GetThenPin();
	UEdGraphPin* CallFinishActor = CallFinishSpawnNode->FindPin(AdventurePtrParam);
	UEdGraphPin* CallFinishResult = CallFinishSpawnNode->GetReturnValuePin();

	// Move 'then' connection from spawn node to 'finish spawn'
	CompilerContext.MovePinLinksToIntermediate(*SpawnNodeThen, *CallFinishThen);

	// Connect output actor from 'begin' to 'finish'
	CallBeginResult->MakeLinkTo(CallFinishActor);

	// Move result connection from spawn node to 'finish spawn'
	CallFinishResult->PinType = SpawnNodeResult->PinType; // Copy type so it uses the right actor subclass
	CompilerContext.MovePinLinksToIntermediate(*SpawnNodeResult, *CallFinishResult);
	
	UEdGraphPin* LastThen = FKismetCompilerUtilities::GenerateAssignmentNodes(CompilerContext, SourceGraph, CallBeginSpawnNode, SpawnNode, CallBeginResult, ClassToSpawn );

	// Make exec connection between 'then' on last node and 'finish'
	LastThen->MakeLinkTo(CallFinishExec);

	// Break any links to the expanded node
	SpawnNode->BreakAllNodeLinks();
}

#undef LOCTEXT_NAMESPACE

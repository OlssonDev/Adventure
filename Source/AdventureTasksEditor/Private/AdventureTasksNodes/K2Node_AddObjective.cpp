// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_AddObjective.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintFunctionNodeSpawner.h"
#include "K2Node_AppendObjective.h"
#include "K2Node_IfThenElse.h"
#include "K2Node_TemporaryVariable.h"
#include "KismetCompiler.h"
#include "BlueprintAssets/AdventureAssetBlueprint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Nodes/AdventureTask_AddObjective.h"

#define LOCTEXT_NAMESPACE "UK2Node_AddObjective"

const FName UK2Node_AddObjective::NAME_ObjectivesArray(TEXT("NewObjectives"));

UK2Node_AddObjective::UK2Node_AddObjective(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (HasAnyFlags(RF_ClassDefaultObject) == true)
	{
		UK2Node_LatentAdventureTaskCall::RegisterSpecializedTaskNodeClass(GetClass());
	}
}

bool UK2Node_AddObjective::IsHandling(TSubclassOf<UAdventureTask> TaskClass) const
{
	return TaskClass && TaskClass->IsChildOf(UAdventureTask_AddObjective::StaticClass());
}

void UK2Node_AddObjective::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	struct GetMenuActions_Utils
	{
		static void SetNodeFunc(UEdGraphNode* NewNode, bool /*bIsTemplateNode*/, TWeakObjectPtr<UFunction> FunctionPtr)
		{
			UK2Node_AddObjective* AsyncTaskNode = CastChecked<UK2Node_AddObjective>(NewNode);
			if (FunctionPtr.IsValid())
			{
				UFunction* Func = FunctionPtr.Get();
				FObjectProperty* ReturnProp = CastFieldChecked<FObjectProperty>(Func->GetReturnProperty());
						
				AsyncTaskNode->ProxyFactoryFunctionName = Func->GetFName();
				AsyncTaskNode->ProxyFactoryClass        = Func->GetOuterUClass();
				AsyncTaskNode->ProxyClass               = ReturnProp->PropertyClass;
			}
		}
	};

	UClass* NodeClass = GetClass();
	ActionRegistrar.RegisterClassFactoryActions<UAdventureTask_AddObjective>(FBlueprintActionDatabaseRegistrar::FMakeFuncSpawnerDelegate::CreateLambda([NodeClass](const UFunction* FactoryFunc)->UBlueprintNodeSpawner*
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintFunctionNodeSpawner::Create(FactoryFunc);
		check(NodeSpawner != nullptr);
		NodeSpawner->NodeClass = NodeClass;

		TWeakObjectPtr<UFunction> FunctionPtr = MakeWeakObjectPtr(const_cast<UFunction*>(FactoryFunc));
		NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(GetMenuActions_Utils::SetNodeFunc, FunctionPtr);

		return NodeSpawner;
	}) );
}

void UK2Node_AddObjective::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	Super::ReallocatePinsDuringReconstruction(OldPins);
	SetArrayInputVisibility(bUseArray);
}

FSlateIcon UK2Node_AddObjective::GetIconAndTint(FLinearColor& OutColor) const
{
	return FSlateIcon("EditorStyle", "ClassIcon.StringTable");
}

bool UK2Node_AddObjective::IsCompatibleWithGraph(UEdGraph const* TargetGraph) const
{
	bool const bIsNotAdventure = TargetGraph->GetOuter()->IsA(UAdventureAssetBlueprint::StaticClass());
	return bIsNotAdventure && Super::IsCompatibleWithGraph(TargetGraph);
}

void UK2Node_AddObjective::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();
	
	CreateObjectivesArray();
	
	if (NumAdditionalInputs == 0)
	{
		AddInputPin();
	}
	else
	{
		UserCreatedObjectivePins.Empty();
		for (int32 i = 0; i < NumAdditionalInputs; ++i)
		{
			AddInputPin_Internal(i);
		}
	}
}

void UK2Node_AddObjective::CreateObjectivesArray()
{
	UEdGraphPin* ObjectivesArrayPin = FindPin(NAME_ObjectivesArray);
	
	if (ObjectivesArrayPin == nullptr)
	{
		FCreatePinParams PinParams;
		PinParams.ContainerType = EPinContainerType::Array;
		ObjectivesArrayPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FAdventureObjectiveEntry::StaticStruct(), NAME_ObjectivesArray, PinParams);
	}

	ObjectivesArrayPin->bHidden = !bUseArray;
}


bool UK2Node_AddObjective::CanRemoveInputPin(const UEdGraphPin* PinToRemove) const
{
	return UserCreatedObjectivePins.Contains(PinToRemove->PinName) && UserCreatedObjectivePins.Num() > 1;
}

void UK2Node_AddObjective::GetNodeContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetNodeContextMenuActions(Menu, Context);

	if (!Context->bIsDebugging)
	{
		FToolMenuSection& Section = Menu->AddSection(FName("ConstructAdventureObjective_Operator"), LOCTEXT("ConstructAdventureObjective_Operator", "Add Objective Node"));
		if (Context->Pin)
		{
			if(CanRemoveInputPin(Context->Pin))
			{
				Section.AddMenuEntry(
					"RemoveObjectivePin",
					LOCTEXT("RemoveObjectivePin", "Remove Objective Pin"),
					LOCTEXT("RemoveObjectivePinTooltip", "Remove this objective pin"),
					FSlateIcon(),
					FUIAction(
						FExecuteAction::CreateUObject(const_cast<UK2Node_AddObjective*>(this), &UK2Node_AddObjective::RemoveInputPin, const_cast<UEdGraphPin*>(Context->Pin))
					)
				);
			}
		}
		
		Section.AddMenuEntry(
			"ToggleInput",
			LOCTEXT("RemoveObjectivePin", "Toggle Objective Type Input"),
			LOCTEXT("RemoveObjectivePinTooltip", "Toggles between singles and array input for objectives"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateUObject(const_cast<UK2Node_AddObjective*>(this), &UK2Node_AddObjective::ChangeInputMode)
			)
		);
	}
}

void UK2Node_AddObjective::RemoveInputPin(UEdGraphPin* Pin)
{
	if(CanRemoveInputPin(Pin))
	{
		FScopedTransaction Transaction( LOCTEXT("RemovePinTx", "RemovePin") );
		Modify();
		
		if (RemovePin(Pin))
		{
			TArray<FName> NewObjectivePinNames;
			--NumAdditionalInputs;

			int32 NameIndex = 0;
			for (int32 PinIndex = 0; PinIndex < Pins.Num(); ++PinIndex)
			{
				UEdGraphPin* LocalPin = Pins[PinIndex];
				if(LocalPin && CanRemoveInputPin(LocalPin))
				{
					const FName PinName = GetPinName(NameIndex);
					if (PinName != LocalPin->PinName)
					{
						LocalPin->Modify();
						LocalPin->PinName = PinName;
					}
					NewObjectivePinNames.Add(PinName);
					NameIndex++;
				}
			}
			UserCreatedObjectivePins = NewObjectivePinNames;
			FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(GetBlueprint());
		}
	}
}

void UK2Node_AddObjective::ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
    UK2Node::ExpandNode(CompilerContext, SourceGraph);

	const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();
	check(SourceGraph && Schema);
	bool bIsErrorFree = true;

	// Create a call to factory the proxy object
	UK2Node_CallFunction* const CallCreateProxyObjectNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallCreateProxyObjectNode->FunctionReference.SetExternalMember(ProxyFactoryFunctionName, ProxyFactoryClass);
	CallCreateProxyObjectNode->AllocateDefaultPins();
	if (CallCreateProxyObjectNode->GetTargetFunction() == nullptr)
	{
		const FText ClassName = ProxyFactoryClass ? FText::FromString(ProxyFactoryClass->GetName()) : LOCTEXT("MissingClassString", "Unknown Class");
		const FString FormattedMessage = FText::Format(
			LOCTEXT("AsyncTaskErrorFmt", "BaseAsyncTask: Missing function {0} from class {1} for async task @@"),
			FText::FromString(ProxyFactoryFunctionName.GetPlainNameString()),
			ClassName
		).ToString();

		CompilerContext.MessageLog.Error(*FormattedMessage, this);
		return;
	}

	bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(UEdGraphSchema_K2::PN_Execute), *CallCreateProxyObjectNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute)).CanSafeConnect();

	for (UEdGraphPin* CurrentPin : Pins)
	{
		if (FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Input) && !UserCreatedObjectivePins.Contains(CurrentPin->PinName) && CurrentPin != FindPin(NAME_ObjectivesArray))
		{
			UEdGraphPin* DestPin = CallCreateProxyObjectNode->FindPin(CurrentPin->PinName); // match function inputs, to pass data to function from CallFunction node
			bIsErrorFree &= DestPin && CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *DestPin).CanSafeConnect();
		}
	}

	UEdGraphPin* const ProxyObjectPin = CallCreateProxyObjectNode->GetReturnValuePin();
	check(ProxyObjectPin);
	UEdGraphPin* OutputAsyncTaskProxy = FindPin(FBaseAsyncTaskHelper::GetAsyncTaskProxyName());
	bIsErrorFree &= !OutputAsyncTaskProxy || CompilerContext.MovePinLinksToIntermediate(*OutputAsyncTaskProxy, *ProxyObjectPin).CanSafeConnect();

	bIsErrorFree &= ExpandDefaultToSelfPin(CompilerContext, SourceGraph, CallCreateProxyObjectNode);		

	// GATHER OUTPUT PARAMETERS AND PAIR THEM WITH LOCAL VARIABLES
	TArray<FBaseAsyncTaskHelper::FOutputPinAndLocalVariable> VariableOutputs;
	bool bPassedFactoryOutputs = false;
	for (UEdGraphPin* CurrentPin : Pins)
	{
		if ((OutputAsyncTaskProxy != CurrentPin) && FBaseAsyncTaskHelper::ValidDataPin(CurrentPin, EGPD_Output))
		{
			if (!bPassedFactoryOutputs)
			{
				UEdGraphPin* DestPin = CallCreateProxyObjectNode->FindPin(CurrentPin->PinName);
				bIsErrorFree &= DestPin && CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *DestPin).CanSafeConnect();
			}
			else
			{
				const FEdGraphPinType& PinType = CurrentPin->PinType;
				UK2Node_TemporaryVariable* TempVarOutput = CompilerContext.SpawnInternalVariable(
					this, PinType.PinCategory, PinType.PinSubCategory, PinType.PinSubCategoryObject.Get(), PinType.ContainerType, PinType.PinValueType);
				bIsErrorFree &= TempVarOutput->GetVariablePin() && CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *TempVarOutput->GetVariablePin()).CanSafeConnect();
				VariableOutputs.Add(FBaseAsyncTaskHelper::FOutputPinAndLocalVariable(CurrentPin, TempVarOutput));
			}
		}
		else if (!bPassedFactoryOutputs && CurrentPin && CurrentPin->Direction == EGPD_Output)
		{
			// the first exec that isn't the node's then pin is the start of the asyc delegate pins
			// once we hit this point, we've iterated beyond all outputs for the factory function
			bPassedFactoryOutputs = (CurrentPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec) && (CurrentPin->PinName != UEdGraphSchema_K2::PN_Then);
		}
	}

	// FOR EACH DELEGATE DEFINE EVENT, CONNECT IT TO DELEGATE AND IMPLEMENT A CHAIN OF ASSIGMENTS
	UEdGraphPin* LastThenPin = CallCreateProxyObjectNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);

	UK2Node_CallFunction* IsValidFuncNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	const FName IsValidFuncName = GET_FUNCTION_NAME_CHECKED(UKismetSystemLibrary, IsValid);
	IsValidFuncNode->FunctionReference.SetExternalMember(IsValidFuncName, UKismetSystemLibrary::StaticClass());
	IsValidFuncNode->AllocateDefaultPins();
	UEdGraphPin* IsValidInputPin = IsValidFuncNode->FindPinChecked(TEXT("Object"));

	bIsErrorFree &= Schema->TryCreateConnection(ProxyObjectPin, IsValidInputPin);

	UK2Node_IfThenElse* ValidateProxyNode = CompilerContext.SpawnIntermediateNode<UK2Node_IfThenElse>(this, SourceGraph);
	ValidateProxyNode->AllocateDefaultPins();
	bIsErrorFree &= Schema->TryCreateConnection(IsValidFuncNode->GetReturnValuePin(), ValidateProxyNode->GetConditionPin());

	bIsErrorFree &= Schema->TryCreateConnection(LastThenPin, ValidateProxyNode->GetExecPin());
	LastThenPin = ValidateProxyNode->GetThenPin();

	bIsErrorFree &= HandleDelegates(VariableOutputs, ProxyObjectPin, LastThenPin, SourceGraph, CompilerContext);

	if (CallCreateProxyObjectNode->FindPinChecked(UEdGraphSchema_K2::PN_Then) == LastThenPin)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("MissingDelegateProperties", "BaseAsyncTask: Proxy has no delegates defined. @@").ToString(), this);
		return;
	}

	UK2Node_CallFunction* const CallPopulateObjectiveProxyObjectNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallPopulateObjectiveProxyObjectNode->FunctionReference.SetExternalMember(GET_FUNCTION_NAME_CHECKED(UAdventureTask_AddObjective, PopulateObjective), UAdventureTask_AddObjective::StaticClass());
	CallPopulateObjectiveProxyObjectNode->AllocateDefaultPins();
	
	bIsErrorFree &= Schema->TryCreateConnection(LastThenPin, CallPopulateObjectiveProxyObjectNode->GetExecPin());
	
	UEdGraphPin* PopulateObjectiveSelfPin = Schema->FindSelfPin(*CallPopulateObjectiveProxyObjectNode, EGPD_Input);
	bIsErrorFree &= Schema->TryCreateConnection(ProxyObjectPin, PopulateObjectiveSelfPin);

	LastThenPin = CallPopulateObjectiveProxyObjectNode->GetThenPin();
	
	if (!bUseArray)
	{
		UEdGraphPin* LastOutPin = nullptr;

		for(int32 PinIndex = 0; PinIndex < Pins.Num(); PinIndex++)
		{
			UEdGraphPin* CurrentPin = Pins[PinIndex];
			
			if(!UserCreatedObjectivePins.Contains(CurrentPin->PinName))
			{
				continue;
			}

			UK2Node_AppendObjective* AppendObjectiveNode = SourceGraph->CreateIntermediateNode<UK2Node_AppendObjective>();
			AppendObjectiveNode->AllocateDefaultPins();
			CompilerContext.MessageLog.NotifyIntermediateObjectCreation(AppendObjectiveNode, this);
			
			if(LastOutPin)
			{
				Schema->TryCreateConnection(LastOutPin, AppendObjectiveNode->GetObjectiveArrayPin());
			}
			
			CompilerContext.MovePinLinksToIntermediate(*CurrentPin, *AppendObjectiveNode->GetObjectivePin());
			LastOutPin = AppendObjectiveNode->GetReturnValuePin();
		}

		if (LastOutPin)
		{
			Schema->TryCreateConnection(LastOutPin, CallPopulateObjectiveProxyObjectNode->FindPin(NAME_ObjectivesArray));	
		}
	}
	else
	{
		CompilerContext.MovePinLinksToIntermediate(*FindPin(NAME_ObjectivesArray), *CallPopulateObjectiveProxyObjectNode->FindPin(NAME_ObjectivesArray));
	}
	
	// Create a call to activate the proxy object if necessary
	if (ProxyActivateFunctionName != NAME_None)
	{
		UK2Node_CallFunction* const CallActivateProxyObjectNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		CallActivateProxyObjectNode->FunctionReference.SetExternalMember(ProxyActivateFunctionName, ProxyClass);
		CallActivateProxyObjectNode->AllocateDefaultPins();

		// Hook up the self connection
		UEdGraphPin* ActivateCallSelfPin = Schema->FindSelfPin(*CallActivateProxyObjectNode, EGPD_Input);
		check(ActivateCallSelfPin);

		bIsErrorFree &= Schema->TryCreateConnection(ProxyObjectPin, ActivateCallSelfPin);

		// Hook the activate node up in the exec chain
		UEdGraphPin* ActivateExecPin = CallActivateProxyObjectNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
		UEdGraphPin* ActivateThenPin = CallActivateProxyObjectNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);

		bIsErrorFree &= Schema->TryCreateConnection(LastThenPin, ActivateExecPin);

		LastThenPin = ActivateThenPin;
	}

	// Move the connections from the original node then pin to the last internal then pin

	UEdGraphPin* OriginalThenPin = FindPin(UEdGraphSchema_K2::PN_Then);

	if (OriginalThenPin)
	{
		bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*OriginalThenPin, *LastThenPin).CanSafeConnect();
	}
	bIsErrorFree &= CompilerContext.CopyPinLinksToIntermediate(*LastThenPin, *ValidateProxyNode->GetElsePin()).CanSafeConnect();

	if (!bIsErrorFree)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("InternalConnectionError", "BaseAsyncTask: Internal connection error. @@").ToString(), this);
	}

	// Make sure we caught everything
	BreakAllNodeLinks();
}

void UK2Node_AddObjective::AddInputPin()
{
	if (NumAdditionalInputs <= TCHAR('Z') - TCHAR('A'))
	{
		AddInputPin_Internal(NumAdditionalInputs);
		NumAdditionalInputs++;
	}
}

bool UK2Node_AddObjective::CanAddPin() const
{
	return true;
}

void UK2Node_AddObjective::AddInputPin_Internal(int32 Index)
{
	UEdGraphPin* Pin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, FAdventureObjectiveEntry::StaticStruct(), GetPinName(Index));
	UserCreatedObjectivePins.Add(Pin->PinName);
}

void UK2Node_AddObjective::ChangeInputMode()
{
	if (!bUseArray)
	{
		SetArrayInputVisibility(true);
	}
	else
	{
		ResetToDefaultInput();
	}
	
	bUseArray = !bUseArray;
	ReconstructNode();
}

void UK2Node_AddObjective::ResetToDefaultInput()
{
	SetArrayInputVisibility(false);
	SetUserCreatedPinsVisibility(true);
}

void UK2Node_AddObjective::SetUserCreatedPinsVisibility(bool IsVisible)
{
	for (const FName PinName : UserCreatedObjectivePins)
	{
		if (UEdGraphPin* FoundPin = FindPin(PinName))
		{
			FoundPin->SafeSetHidden(!IsVisible);
			
			if (IsVisible == false)
			{
				FoundPin->BreakAllPinLinks();
			}
		}
	}
}

void UK2Node_AddObjective::SetArrayInputVisibility(bool bVisible)
{
	if (UEdGraphPin* Pin = FindPin(NAME_ObjectivesArray))
	{
		Pin->bHidden = !bVisible;

		if (bVisible == false)
		{
			Pin->BreakAllPinLinks();
		}
	}

	SetUserCreatedPinsVisibility(!bVisible);
}


FName UK2Node_AddObjective::GetPinName(int32 Index)
{
	return FName(*FString::Printf(TEXT("Objective %s"), *FString::Chr(static_cast<TCHAR>('A') + Index)));
}

#undef LOCTEXT_NAMESPACE

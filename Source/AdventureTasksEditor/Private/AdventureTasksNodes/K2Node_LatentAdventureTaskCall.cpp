// Fill out your copyright notice in the Description page of Project Settings.


#include "K2Node_LatentAdventureTaskCall.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintFunctionNodeSpawner.h"
#include "BlueprintNodeSpawner.h"
#include "AdventureTasks/Public/Nodes/AdventureTask.h"
#include "BlueprintAssets/AdventureAssetBlueprint.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "K2Node_LatentAdventureTaskCall"

TArray<TWeakObjectPtr<UClass> > UK2Node_LatentAdventureTaskCall::NodeClasses;

UK2Node_LatentAdventureTaskCall::UK2Node_LatentAdventureTaskCall(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ProxyActivateFunctionName = GET_FUNCTION_NAME_CHECKED(UAdventureTask, ActivateTask);
}

void UK2Node_LatentAdventureTaskCall::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	struct GetMenuActions_Utils
	{
		static void SetNodeFunc(UEdGraphNode* NewNode, bool /*bIsTemplateNode*/, TWeakObjectPtr<UFunction> FunctionPtr)
		{
			UK2Node_LatentAdventureTaskCall* AsyncTaskNode = CastChecked<UK2Node_LatentAdventureTaskCall>(NewNode);
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
	ActionRegistrar.RegisterClassFactoryActions<UAdventureTask>( FBlueprintActionDatabaseRegistrar::FMakeFuncSpawnerDelegate::CreateLambda([NodeClass](const UFunction* FactoryFunc)->UBlueprintNodeSpawner*
	{
		UBlueprintNodeSpawner* NodeSpawner = nullptr;
		UClass* FuncClass = FactoryFunc->GetOwnerClass();
		if (!UK2Node_LatentAdventureTaskCall::HasDedicatedNodeClass(FuncClass))
		{
			NodeSpawner = UBlueprintFunctionNodeSpawner::Create(FactoryFunc);
			check(NodeSpawner != nullptr);
			NodeSpawner->NodeClass = NodeClass;

			TWeakObjectPtr<UFunction> FunctionPtr = MakeWeakObjectPtr(const_cast<UFunction*>(FactoryFunc));
			NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(GetMenuActions_Utils::SetNodeFunc, FunctionPtr);
		}
		return NodeSpawner;
	}) );
}

void UK2Node_LatentAdventureTaskCall::RegisterSpecializedTaskNodeClass(TSubclassOf<UK2Node_LatentAdventureTaskCall> NodeClass)
{
	if (NodeClass)
	{
		NodeClasses.AddUnique(*NodeClass);
	}
}

bool UK2Node_LatentAdventureTaskCall::HasDedicatedNodeClass(TSubclassOf<UAdventureTask> TaskClass)
{
	for (const TWeakObjectPtr<UClass>& NodeClass : NodeClasses)
	{
		if (NodeClass.IsValid())
		{
			UK2Node_LatentAdventureTaskCall* NodeCDO = NodeClass->GetDefaultObject<UK2Node_LatentAdventureTaskCall>();
			if (NodeCDO && NodeCDO->IsHandling(TaskClass))
			{
				return true;
			}
		}
	}
	return false;
}

#undef LOCTEXT_NAMESPACE

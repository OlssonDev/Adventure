// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node_BaseAsyncTask.h"
#include "K2Node_LatentAdventureTaskCall.generated.h"

class UAdventureTask;
UCLASS()
class ADVENTURETASKSEDITOR_API UK2Node_LatentAdventureTaskCall : public UK2Node_BaseAsyncTask
{
	GENERATED_BODY()

	public:
	
	UK2Node_LatentAdventureTaskCall(const FObjectInitializer& ObjectInitializer);

	// UEdGraphNode interface
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	// End of UEdGraphNode interface

	public:

	static void RegisterSpecializedTaskNodeClass(TSubclassOf<UK2Node_LatentAdventureTaskCall> NodeClass);

	protected:
	
	static bool HasDedicatedNodeClass(TSubclassOf<UAdventureTask> TaskClass);
	virtual bool IsHandling(TSubclassOf<UAdventureTask> TaskClass) const { return true; }

	private:
	
	static TArray<TWeakObjectPtr<UClass>> NodeClasses;
};

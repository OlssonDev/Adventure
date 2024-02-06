// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureTask.h"
#include "FAdventureGraphNodeMinimal.h"
#include "AdventureTask_ListenObjectiveEntryChanges.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveEntryChanged, FAdventureObjectiveEntry, ObjectiveEntry, int32, Index);

UCLASS()
class ADVENTURETASKS_API UAdventureTask_ListenObjectiveEntryChanges : public UAdventureTask
{
	GENERATED_BODY()

	public:

	//UAdventureTask interface implementation
	virtual void ActivateTask() override;
	//End of implementation

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), DisplayName = "Listen For Objective Changes", Category = "Adventure | Tasks")
	static UAdventureTask_ListenObjectiveEntryChanges* ListenObjectiveEntryChanges(UAdventureAsset* Adventure);

	public:

	void OnObjectiveEntryChanged(FAdventureObjectiveEntry Entry, int32 Index);

	public:

	UPROPERTY(BlueprintAssignable)
	FOnObjectiveEntryChanged OnChanged;
	
};

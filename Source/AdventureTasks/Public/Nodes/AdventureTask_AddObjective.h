// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureTask.h"
#include "AdventureTask_ListenObjectiveEntryChanges.h"
#include "AdventureTask_AddObjective.generated.h"

UCLASS()
class ADVENTURETASKS_API UAdventureTask_AddObjective : public UAdventureTask
{
	GENERATED_BODY()

	protected:

	//UAdventureTask interface implementation
	virtual void ActivateTask() override;
	//End of implementation

	public:

	//Add a new objective or multiple objectives to the Adventure.
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Adventure Task | Objectives", meta =(WorldContext=AdventureContextObject))
	static UAdventureTask_AddObjective* AddObjective(UObject* AdventureContextObject);

	//Just a template for the UK2Node_AddObjective
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	void PopulateObjective(TArray<FAdventureObjectiveEntry> NewObjectives);

	//Get the unique ID for this objective 
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure Task | Objectives", meta = (CompactNodeTitle=UniqueID))
	int64 GetObjectiveUniqueID()
	{
		return ObjectiveUniqueID;
	}

	private:

	void ObjectiveStepCompleted(FAdventureObjectiveEntry CompletedStep);
	void ObjectiveConditionTagsUpdated(FAdventureObjectiveEntry UpdatedEntry, int32 Index);
	void ObjectiveCompleted(int64 ObjectiveUniqueID);

	bool IsMatchingID(FAdventureObjectiveEntry Entry)
	{
		return Entry.ObjectiveUniqueID == ObjectiveUniqueID;
	};

	public:

	TArray<FAdventureObjectiveEntry> NewObjectives;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FK2OnObjectiveStepCompleted, FAdventureObjectiveEntry, ObjectiveStep);
	UPROPERTY(BlueprintAssignable)
	FK2OnObjectiveStepCompleted OnObjectiveStepCompleted;
	
	UPROPERTY(BlueprintAssignable)
	FK2OnObjectiveStepCompleted OnConditionTagsModified;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FK2OnObjectiveListCompleted);
	UPROPERTY(BlueprintAssignable)
	FK2OnObjectiveListCompleted OnObjectiveListFinished;

	private:

	int64 ObjectiveUniqueID;
	
};

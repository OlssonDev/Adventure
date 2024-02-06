// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/AdventureTask_AddObjective.h"

#include "AdventureAsset.h"

void UAdventureTask_AddObjective::ActivateTask()
{
	Super::ActivateTask();

	UAdventureAsset* OwningAdventure = CastChecked<UAdventureAsset>(GetOuter());
	OwningAdventure->ObjectiveList.ObjectiveStepCompleted.AddUObject(this, &ThisClass::ObjectiveStepCompleted);
	OwningAdventure->ObjectiveList.ObjectiveConditionTagsUpdated.AddUObject(this, &ThisClass::ObjectiveConditionTagsUpdated);
	OwningAdventure->ObjectiveList.ObjectiveCompleted.AddUObject(this, &ThisClass::ObjectiveCompleted);
	OwningAdventure->AddNewObjectiveList(NewObjectives, ObjectiveUniqueID);
	NewObjectives.Empty();
}

UAdventureTask_AddObjective* UAdventureTask_AddObjective::AddObjective(UObject* AdventureContextObject)
{
	if (IsValid(AdventureContextObject))
	{
		return NewObject<UAdventureTask_AddObjective>(AdventureContextObject);
	}
	return nullptr;
}

void UAdventureTask_AddObjective::PopulateObjective(TArray<FAdventureObjectiveEntry> InNewObjectives)
{
	NewObjectives = InNewObjectives;
}

void UAdventureTask_AddObjective::ObjectiveStepCompleted(FAdventureObjectiveEntry CompletedStep)
{
	if (IsMatchingID(CompletedStep))
	{
		OnObjectiveStepCompleted.Broadcast(CompletedStep);
	}
}

void UAdventureTask_AddObjective::ObjectiveConditionTagsUpdated(FAdventureObjectiveEntry UpdatedEntry, int32 Index)
{
	if (IsMatchingID(UpdatedEntry))
	{
		OnConditionTagsModified.Broadcast(UpdatedEntry);
	}
}

void UAdventureTask_AddObjective::ObjectiveCompleted(int64 InObjectiveUniqueID)
{
	if (InObjectiveUniqueID == ObjectiveUniqueID)
	{
		OnObjectiveListFinished.Broadcast();
		DeactivateTask();
	}
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/AdventureTask_ListenObjectiveEntryChanges.h"
#include "AdventureAsset.h"

void UAdventureTask_ListenObjectiveEntryChanges::ActivateTask()
{
	Super::ActivateTask();
	AdventureAsset->ObjectiveList.ObjectiveEntryUpdated.AddUObject(this, &UAdventureTask_ListenObjectiveEntryChanges::OnObjectiveEntryChanged);
}

UAdventureTask_ListenObjectiveEntryChanges* UAdventureTask_ListenObjectiveEntryChanges::ListenObjectiveEntryChanges(UAdventureAsset* Adventure)
{
	if (IsValid(Adventure))
	{
		UAdventureTask_ListenObjectiveEntryChanges* ListenObjectiveEntryChangesTask = NewObject<UAdventureTask_ListenObjectiveEntryChanges>(Adventure, StaticClass());
		ListenObjectiveEntryChangesTask->AdventureAsset = Adventure;
		return ListenObjectiveEntryChangesTask;
	}

	return nullptr;
}

void UAdventureTask_ListenObjectiveEntryChanges::OnObjectiveEntryChanged(FAdventureObjectiveEntry Entry, int32 Index)
{
	OnChanged.Broadcast(Entry, Index);
}

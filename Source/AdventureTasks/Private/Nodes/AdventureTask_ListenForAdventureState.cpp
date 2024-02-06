// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/AdventureTask_ListenForAdventureState.h"

#include "AdventureAsset.h"

void UAdventureTask_ListenForAdventureState::ActivateTask()
{
	Super::ActivateTask();
	AdventureAsset->OnAdventureStateModified.AddUObject(this, &UAdventureTask_ListenForAdventureState::NewState);
}

void UAdventureTask_ListenForAdventureState::NewState(TEnumAsByte<EAdventureState> NewAdventureState)
{
	OnStateChanged.Broadcast(NewAdventureState);
}

UAdventureTask_ListenForAdventureState* UAdventureTask_ListenForAdventureState::ListenForAdventureStateChanges(UAdventureAsset* Adventure)
{
	if (IsValid(Adventure))
	{
		UAdventureTask_ListenForAdventureState* ListenForAdventureState = NewObject<UAdventureTask_ListenForAdventureState>(Adventure);
		ListenForAdventureState->AdventureAsset = Adventure;
		return ListenForAdventureState;
	}

	return nullptr;
}

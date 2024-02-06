// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/AdventureTask_ListenForSaveStartFinished.h"

#include "AdventureComponent.h"
#include "AdventureGlobals.h"

void UAdventureTask_ListenForSaveStartFinished::ActivateTask()
{
	Super::ActivateTask();

	AdventureComponent->SavingAdventureComponent.AddUObject(this, &ThisClass::AdventureComponentPreSave);
	AdventureComponent->FinishedSavingAdventureComponent.AddUObject(this, &ThisClass::AdventureComponentSaveFinished);
}

UAdventureTask_ListenForSaveStartFinished* UAdventureTask_ListenForSaveStartFinished::ListenForSaveStartAndFinish(AActor* AdventureComponentOwner)
{
	if (UAdventureComponent* AdventureComponent = UAdventureGlobals::GetAdventureComponentFromActor(AdventureComponentOwner, true))
	{
		UAdventureTask_ListenForSaveStartFinished* ListenForAdventureState = NewObject<UAdventureTask_ListenForSaveStartFinished>(AdventureComponentOwner);
		ListenForAdventureState->AdventureComponent = AdventureComponent;
		return ListenForAdventureState;
	}

	return nullptr;
}

void UAdventureTask_ListenForSaveStartFinished::AdventureComponentPreSave()
{
	OnPreSave.Broadcast();
}

void UAdventureTask_ListenForSaveStartFinished::AdventureComponentSaveFinished()
{
	OnSaveFinished.Broadcast();
}

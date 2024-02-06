// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/AdventureTask_ListenForLoadStartFinished.h"
#include "AdventureComponent.h"
#include "AdventureGlobals.h"

void UAdventureTask_ListenForLoadStartFinished::ActivateTask()
{
	Super::ActivateTask();
	PreLoadHandle = AdventureComponent->LoadingAdventureComponent.AddUObject(this, &ThisClass::AdventureComponentPreLoad);
	LoadFinishedHandle = AdventureComponent->FinishedLoadingAdventureComponent.AddUObject(this, &ThisClass::AdventureComponentLoadFinished);
}

void UAdventureTask_ListenForLoadStartFinished::DeactivateTask()
{
	Super::DeactivateTask();
	AdventureComponent->LoadingAdventureComponent.Remove(PreLoadHandle);
	AdventureComponent->FinishedLoadingAdventureComponent.Remove(LoadFinishedHandle);
}

UAdventureTask_ListenForLoadStartFinished* UAdventureTask_ListenForLoadStartFinished::ListenForLoadStartAndFinish(AActor* AdventureComponentOwner)
{
	if (UAdventureComponent* AdventureComponent = UAdventureGlobals::GetAdventureComponentFromActor(AdventureComponentOwner, true))
	{
		UAdventureTask_ListenForLoadStartFinished* ListenForAdventureState = NewObject<UAdventureTask_ListenForLoadStartFinished>(AdventureComponentOwner);
		ListenForAdventureState->AdventureComponent = AdventureComponent;
		return ListenForAdventureState;
	}

	return nullptr;
}

void UAdventureTask_ListenForLoadStartFinished::AdventureComponentPreLoad()
{
	OnPreLoad.Broadcast();
}

void UAdventureTask_ListenForLoadStartFinished::AdventureComponentLoadFinished()
{
	OnLoadFinished.Broadcast();
}

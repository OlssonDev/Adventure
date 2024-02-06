// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/AdventureTask_ListenActiveAdventuresModified.h"
#include "AdventureComponent.h"
#include "AdventureGlobals.h"

void UAdventureTask_ListenActiveAdventuresModified::ActivateTask()
{
	Super::ActivateTask();

	AdventureComponent->ActiveAdventuresModified.AddUObject(this, &ThisClass::OnActiveAdventuresModified);
}

void UAdventureTask_ListenActiveAdventuresModified::OnActiveAdventuresModified(TArray<UAdventureAsset*> ActiveAdventures)
{
	OnChanged.Broadcast(ActiveAdventures);
}

UAdventureTask_ListenActiveAdventuresModified* UAdventureTask_ListenActiveAdventuresModified::ListenForActiveAdventureChanges(AActor* AdventureComponentOwner)
{
	if (UAdventureComponent* AdventureComponent = UAdventureGlobals::GetAdventureComponentFromActor(AdventureComponentOwner, true))
	{
		UAdventureTask_ListenActiveAdventuresModified* ListenActiveAdventuresModified = NewObject<UAdventureTask_ListenActiveAdventuresModified>(AdventureComponentOwner);
		ListenActiveAdventuresModified->AdventureComponent = AdventureComponent;
		return ListenActiveAdventuresModified;
	}

	return nullptr;
}

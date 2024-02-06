// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/AdventureTask_LoadChainedAdventures.h"
#include "AdventureAsset.h"

void UAdventureTask_LoadChainedAdventures::ActivateTask()
{
	Super::ActivateTask();
	
	AdventureAsset->LoadChainedAdventures(FOnChainedAdventuresLoaded::CreateWeakLambda(this, [this](TArray<TSubclassOf<UAdventureAsset>> LoadedChainedAdventures)
	{
		OnLoaded.Broadcast(LoadedChainedAdventures);
	}));
}

UAdventureTask_LoadChainedAdventures* UAdventureTask_LoadChainedAdventures::LoadChainedAdventures(UAdventureAsset* Adventure)
{
	if (IsValid(Adventure))
	{
		UAdventureTask_LoadChainedAdventures* LoadChainedAdventures = NewObject<UAdventureTask_LoadChainedAdventures>(Adventure);
		LoadChainedAdventures->AdventureAsset = Adventure;
		return LoadChainedAdventures;
	}

	return nullptr;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGlobals.h"
#include "AdventureComponent.h"
#include "AdventureInterface.h"
#include "AdventureMacros.h"

#define LOCTEXT_NAMESPACE "AdventureGlobals"

UAdventureComponent* UAdventureGlobals::GetAdventureComponentFromActor(AActor* Actor, bool bSearchForComponent)
{
	check(Actor)
	
	if (!IsValid(Actor))
	{
		ADVENTURE_ERROR("Failed to get Adventure Component from Actor. Reason: Actor reference was invalid.")
		return nullptr;
	}
	
	if (IAdventureInterface* AdventureInterface = Cast<IAdventureInterface>(Actor))
	{
		return AdventureInterface->GetAdventureComponent();
	}

	if (bSearchForComponent)
	{
		if (UAdventureComponent* FoundComponent = Actor->FindComponentByClass<UAdventureComponent>())
		{
			return FoundComponent;
		}
	}

	ADVENTURE_ERROR(FString::Printf(TEXT("Failed to get Adventure Component from Actor. Reason: Couldn't find a Adventure Component in %s"), *Actor->GetClass()->GetName()));
	return nullptr;
}

#undef LOCTEXT_NAMESPACE

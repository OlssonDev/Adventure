// Fill out your copyright notice in the Description page of Project Settings.


#include "Volumes/AdventureStartTriggerVolume.h"

#include "AdventureComponent.h"


AAdventureStartTriggerVolume::AAdventureStartTriggerVolume()
{
	
}

void AAdventureStartTriggerVolume::RunAdventureComponentOperation(UAdventureComponent* AdventureComponent)
{
	check(AdventureComponent)
	AdventureComponent->StartAdventure(AdventureToStart);
}


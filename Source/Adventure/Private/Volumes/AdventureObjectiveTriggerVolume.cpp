// Fill out your copyright notice in the Description page of Project Settings.


#include "Volumes/AdventureObjectiveTriggerVolume.h"

#include "AdventureComponent.h"

AAdventureObjectiveTriggerVolume::AAdventureObjectiveTriggerVolume()
{
	
}

void AAdventureObjectiveTriggerVolume::RunAdventureComponentOperation(UAdventureComponent* AdventureComponent)
{
	check(AdventureComponent)
	AdventureComponent->UpdateObjective(ObjectiveArgument);
}

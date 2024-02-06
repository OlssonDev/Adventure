// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureSaveObject.h"

#include "AdventureMacros.h"
#include "Kismet/GameplayStatics.h"

void UAdventureSaveObject::SaveToDisk()
{
	if (!SlotName.IsEmpty())
	{
		UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
	}
	else
	{
		ADVENTURE_ERROR("Can't Save Adventure Save Object. Reason: Slot name is empty.");
	}
}

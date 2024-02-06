// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintAssets/AdventureAssetBlueprint.h"
#include "AdventureBlueprintGeneratedClass.h"

UAdventureAssetBlueprint::UAdventureAssetBlueprint()
{
	
}

bool UAdventureAssetBlueprint::SupportedByDefaultBlueprintFactory() const
{
	return false;
}

UClass* UAdventureAssetBlueprint::GetBlueprintClass() const
{
	return UAdventureBlueprintGeneratedClass::StaticClass();
}

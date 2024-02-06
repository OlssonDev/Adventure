// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AdventureSaveObject.generated.h"

USTRUCT(BlueprintType)
struct FSavedActiveAdventure
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<class UAdventureAsset> SavedActiveAdventure;

	UPROPERTY(BlueprintReadOnly)
	TArray<uint8> SavedActiveAdventureData;

	FSavedActiveAdventure()
	{
		SavedActiveAdventure = nullptr;
		SavedActiveAdventureData.Empty();
	}

	FSavedActiveAdventure(TSubclassOf<class UAdventureAsset> InSavedActiveAdventure, TArray<uint8> InSavedActiveAdventureData)
	{
		SavedActiveAdventure = InSavedActiveAdventure;
		SavedActiveAdventureData = InSavedActiveAdventureData;
	}
};

UCLASS()
class ADVENTURE_API UAdventureSaveObject : public USaveGame
{
	GENERATED_BODY()

	public:

	//Saves the adventure save object
	UFUNCTION(BlueprintCallable, Category = "Adventutre Save Object")
	void SaveToDisk();

	public:

	UPROPERTY(BlueprintReadOnly, Category = "Adventure Save Object", SaveGame)
	FString SlotName;
	
	UPROPERTY(BlueprintReadOnly, Category = "Adventure Save Object", SaveGame)
	TArray<uint8> AdventureComponentSaveData;

	UPROPERTY(BlueprintReadOnly, Category = "Adventure Save Object", SaveGame)
	TArray<FSavedActiveAdventure> SavedActiveAdventures;
};

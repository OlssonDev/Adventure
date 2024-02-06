// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureTask.h"
#include "AdventureTask_LoadChainedAdventures.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoaded, const TArray<TSubclassOf<class UAdventureAsset>>&, LoadedChainedAdventures);

class UAdventureAsset;

UCLASS()
class ADVENTURETASKS_API UAdventureTask_LoadChainedAdventures : public UAdventureTask
{
	GENERATED_BODY()

	public:

	//UAdventureTask interface implementation
	virtual void ActivateTask() override;
	//End of implementation
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContextObject = "AdventureAsset"), Category = "Adventure | Tasks")
	static UAdventureTask_LoadChainedAdventures* LoadChainedAdventures(UAdventureAsset* Adventure);

	public:
	
	UPROPERTY(BlueprintAssignable)
	FOnLoaded OnLoaded;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureTask.h"
#include "AdventureComponent.h"
#include "AdventureTask_ListenActiveAdventuresModified.generated.h"

class UAdventureComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveAdventuresModified, const TArray<UAdventureAsset*>&, NewAdventure);

UCLASS()
class ADVENTURETASKS_API UAdventureTask_ListenActiveAdventuresModified : public UAdventureTask
{
	GENERATED_BODY()

	public:
	
	//UAdventureTask interface implementation
	virtual void ActivateTask() override;
	//End of implementation

	//Listen for changes made to the Active Adventures array in Adventure Component. Can listen from both server and client.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Adventure | Tasks")
	static UAdventureTask_ListenActiveAdventuresModified* ListenForActiveAdventureChanges(AActor* AdventureComponentOwner);

	private:

	UFUNCTION()
	void OnActiveAdventuresModified(TArray<UAdventureAsset*> ActiveAdventures);

	public:

	UPROPERTY(BlueprintAssignable)
	FOnActiveAdventuresModified OnChanged;

	private:
	
	UPROPERTY()
	TWeakObjectPtr<UAdventureComponent> AdventureComponent;
	
};

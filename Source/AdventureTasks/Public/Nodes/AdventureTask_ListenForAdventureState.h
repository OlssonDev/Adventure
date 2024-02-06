// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureTask.h"
#include "AdventureAsset.h"
#include "FAdventureGraphNodeMinimal.h"
#include "AdventureTask_ListenForAdventureState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAdventureStateModified, TEnumAsByte<EAdventureState>, NewState);

UCLASS()
class ADVENTURETASKS_API UAdventureTask_ListenForAdventureState : public UAdventureTask
{
	GENERATED_BODY()

	public:
	
	//UAdventureTask interface implementation
	virtual void ActivateTask() override;
	//End of implementation

	//Listen for changes made to the Adventure State enum in an Adventure. Can listen from both server and client.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", WorldContextObject = "AdventureAsset"), Category = "Adventure | Tasks")
	static UAdventureTask_ListenForAdventureState* ListenForAdventureStateChanges(UAdventureAsset* Adventure);

	private:

	UFUNCTION()
	void NewState(TEnumAsByte<EAdventureState> NewState);

	public:

	UPROPERTY(BlueprintAssignable)
	FOnAdventureStateModified OnStateChanged;
};

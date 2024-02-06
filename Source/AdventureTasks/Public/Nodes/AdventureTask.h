// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureTask.generated.h"

//Base class for all UBlueprintAsyncActionBase like events.
UCLASS(Abstract, BlueprintType, meta = (ExposedAsyncProxy=AdventureTask))
class ADVENTURETASKS_API UAdventureTask : public UObject
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	virtual void ActivateTask();

	//Unregisters this async task and will get garbage collected
	UFUNCTION(BlueprintCallable)
	virtual void DeactivateTask();

	protected:

	//Register the task so it doesn't get garbage collected
	virtual void RegisterTaskToGameInstance();
	
	//Unregister this task
	virtual void UnRegisterTaskFromGameInstance();

	protected:

	UPROPERTY()
	UGameInstance* OwningGameInstance;

	UPROPERTY()
	TWeakObjectPtr<class UAdventureAsset> AdventureAsset;
};

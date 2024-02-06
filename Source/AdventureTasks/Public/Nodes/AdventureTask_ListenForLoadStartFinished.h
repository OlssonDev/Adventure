// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureMacros.h"
#include "AdventureTask.h"
#include "AdventureTask_ListenForSaveStartFinished.h"
#include "AdventureTask_ListenForLoadStartFinished.generated.h"

class UAdventureComponent;

UCLASS()
class ADVENTURETASKS_API UAdventureTask_ListenForLoadStartFinished : public UAdventureTask
{
	GENERATED_BODY()

public:
	
	//UAdventureTask interface implementation
	virtual void ActivateTask() override;
	virtual void DeactivateTask() override;
	//End of implementation

	//Listen for changes made to the Adventure State enum in an Adventure. Can listen from both server and client.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Adventure | Tasks")
	static UAdventureTask_ListenForLoadStartFinished* ListenForLoadStartAndFinish(AActor* AdventureComponentOwner);

	void AdventureComponentPreLoad();
	void AdventureComponentLoadFinished();

public:

	UPROPERTY()
	TWeakObjectPtr<UAdventureComponent> AdventureComponent;

	UPROPERTY(BlueprintAssignable)
	FSimpleDynamicMulticastDelegate OnPreLoad;

	UPROPERTY(BlueprintAssignable)
	FSimpleDynamicMulticastDelegate OnLoadFinished;

	FDelegateHandle PreLoadHandle;
	FDelegateHandle LoadFinishedHandle;
};

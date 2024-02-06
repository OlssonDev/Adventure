// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureTask.h"
#include "AdventureTask_ListenForSaveStartFinished.generated.h"

class UAdventureComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleDynamicMulticastDelegate);

UCLASS()
class ADVENTURETASKS_API UAdventureTask_ListenForSaveStartFinished : public UAdventureTask
{
	GENERATED_BODY()

	public:
	
	//UAdventureTask interface implementation
	virtual void ActivateTask() override;
	//End of implementation

	//Listen for changes made to the Adventure State enum in an Adventure. Can listen from both server and client.
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "Adventure | Tasks")
	static UAdventureTask_ListenForSaveStartFinished* ListenForSaveStartAndFinish(AActor* AdventureComponentOwner);

	void AdventureComponentPreSave();
	void AdventureComponentSaveFinished();

	public:

	UPROPERTY()
	TWeakObjectPtr<UAdventureComponent> AdventureComponent;

	UPROPERTY(BlueprintAssignable)
	FSimpleDynamicMulticastDelegate OnPreSave;

	UPROPERTY(BlueprintAssignable)
	FSimpleDynamicMulticastDelegate OnSaveFinished;
};

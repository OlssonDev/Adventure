// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureTriggerBoxVolume.h"
#include "AdventureObjectiveTriggerVolume.generated.h"

UCLASS()
class ADVENTURE_API AAdventureObjectiveTriggerVolume : public AAdventureTriggerBoxVolume
{
	GENERATED_BODY()

	public:
	
	AAdventureObjectiveTriggerVolume();

	//AAdventureTriggerBoxVolume interface implementation
	virtual void RunAdventureComponentOperation(UAdventureComponent* AdventureComponent) override;
	//End of implementation

	public:

	//The argument to update once this volume has been overlapped
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Adventure Trigger Volume - Objective Argument")
	FName ObjectiveArgument;
	
};

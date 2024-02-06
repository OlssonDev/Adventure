// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureTriggerBoxVolume.h"
#include "AdventureStartTriggerVolume.generated.h"

UCLASS()
class ADVENTURE_API AAdventureStartTriggerVolume : public AAdventureTriggerBoxVolume
{
	GENERATED_BODY()

	public:
	
	AAdventureStartTriggerVolume();
	
	//AAdventureTriggerBoxVolume interface implementation
	virtual void RunAdventureComponentOperation(UAdventureComponent* AdventureComponent) override;
	//End of implementation

	public:
	
	//Which adventure should start when overlapping this volume
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Adventure Trigger Volume - Class")
	TSubclassOf<class UAdventureAsset> AdventureToStart;
};

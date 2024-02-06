// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AdventureTriggerBoxVolume.generated.h"

UENUM()
enum EAdventureComponentGetterSettings
{
	//Get from overlapping character/pawn
	CharacterOrPawn,
	//Get from overlapping character/pawn player controller
	PlayerController,
	//Get from overlapping character/pawn player state
	PlayerState,
	//Get from Game State
	GameState,
	//Get from Game Mode
	GameMode,
};

class UAdventureComponent;

UCLASS(hidecategories=(Advanced, Collision, Volume, Brush, Attachment, Input, Replication, Rendering, Actor, LOD, Cooking), ConversionRoot, Abstract)
class ADVENTURE_API AAdventureTriggerBoxVolume : public AActor
{
	GENERATED_BODY()

	public:
	
	AAdventureTriggerBoxVolume();

	public:

	UPROPERTY()
	class UBoxComponent* TriggerBoxVolume;

	UPROPERTY()
	class UBillboardComponent* AdventureIconBillboard;

	//Which actor should the volume try to get the Adventure Component from when this volume has been overlapped?
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Adventure Trigger Volume - Getter Settings")
	TEnumAsByte<EAdventureComponentGetterSettings> GetterSettings;

	protected:

	UFUNCTION()
	virtual void OnComponentOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
	virtual void RunAdventureComponentOperation(UAdventureComponent* AdventureComponent) {};
};

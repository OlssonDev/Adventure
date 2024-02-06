// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureAssetActorFactory.h"
#include "AdventureAsset.h"
#include "BlueprintAssets/AdventureAssetBlueprint.h"
#include "Volumes/AdventureStartTriggerVolume.h"

#define LOCTEXT_NAMESPACE "AdventureAssetActorFactory"

UAdventureAssetActorFactory::UAdventureAssetActorFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	DisplayName = LOCTEXT("AdventureAssetDisplayName", "Adventure Asset");
	NewActorClass = AAdventureStartTriggerVolume::StaticClass();
	bUseSurfaceOrientation = true;
}

bool UAdventureAssetActorFactory::CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg)
{
	if (!AssetData.IsValid() || !AssetData.GetClass()->IsChildOf(UAdventureAssetBlueprint::StaticClass()))
	{
		return false;
	}

	return true;
}

void UAdventureAssetActorFactory::PostSpawnActor(UObject* Asset, AActor* NewActor)
{
	Super::PostSpawnActor(Asset, NewActor);

	UAdventureAssetBlueprint* AdventureAsset = CastChecked<UAdventureAssetBlueprint>(Asset);
	AAdventureStartTriggerVolume* AdventureTriggerVolume = CastChecked<AAdventureStartTriggerVolume>( NewActor );
	AdventureTriggerVolume->AdventureToStart = AdventureAsset->GeneratedClass;
}

UObject* UAdventureAssetActorFactory::GetAssetFromActorInstance(AActor* ActorInstance)
{
	check(ActorInstance->IsA(NewActorClass));
	AAdventureStartTriggerVolume* AdventureTrigger = CastChecked<AAdventureStartTriggerVolume>(ActorInstance);
	return AdventureTrigger->AdventureToStart;
}

FQuat UAdventureAssetActorFactory::AlignObjectToSurfaceNormal(const FVector& InSurfaceNormal, const FQuat& ActorRotation) const
{
	return FindActorAlignmentRotation(ActorRotation, FVector(0.f, 0.f, 1.f), InSurfaceNormal);
}

#undef LOCTEXT_NAMESPACE

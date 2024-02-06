// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureComponent.h"
#include "Components/ActorComponent.h"
#include "AdventureSharedComponent.generated.h"

/**
 * Adventure Shared Component is the same functionality as the vanilla Adventure Component, just that all Adventures are shared among all client.
 * The vanilla version shares only the Adventures between the owning client of the Actor.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), DisplayName = "Adventure Shared Component")
class ADVENTURE_API UAdventureSharedComponent : public UAdventureComponent
{
	GENERATED_BODY()

	public:
	
	UAdventureSharedComponent();

	//UObject interface implementation
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	//End of implementation
};

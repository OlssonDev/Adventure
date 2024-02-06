// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureSharedComponent.h"

#include "Net/UnrealNetwork.h"

UAdventureSharedComponent::UAdventureSharedComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAdventureSharedComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	UActorComponent::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ActiveAdventures);
}



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AdventureGlobals.generated.h"


UCLASS()
class ADVENTURE_API UAdventureGlobals : public UObject
{
	GENERATED_BODY()

	public:

	/** Searches the passed in actor for an Adventure Component, will use IAdventureInterface or fall back to a component search */
	static class UAdventureComponent* GetAdventureComponentFromActor(AActor* Actor, bool bSearchForComponent=false);
	
};

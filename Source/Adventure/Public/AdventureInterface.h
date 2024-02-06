// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AdventureInterface.generated.h"

class UAdventureComponent;

UINTERFACE()
class UAdventureInterface : public UInterface
{
	GENERATED_BODY()
};


class ADVENTURE_API IAdventureInterface
{
	GENERATED_BODY()

	public:
	
	//Get the adventure component
	virtual UAdventureComponent* GetAdventureComponent() const;
};

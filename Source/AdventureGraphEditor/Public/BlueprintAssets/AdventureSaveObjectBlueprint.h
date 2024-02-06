// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureSaveObjectBlueprint.generated.h"

UCLASS()
class ADVENTUREGRAPHEDITOR_API UAdventureSaveObjectBlueprint : public UBlueprint
{
	GENERATED_BODY()

	public:
	
	UAdventureSaveObjectBlueprint();
	
	//UBlueprint Interface
	virtual bool SupportedByDefaultBlueprintFactory() const override;
	//End of UBlueprintInterface
};

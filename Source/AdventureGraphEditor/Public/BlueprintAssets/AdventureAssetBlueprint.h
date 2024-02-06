// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureAssetBlueprint.generated.h"

UCLASS(BlueprintType)
class ADVENTUREGRAPHEDITOR_API UAdventureAssetBlueprint : public UBlueprint
{
	GENERATED_BODY()

	public:

	UAdventureAssetBlueprint();
	
	//UBlueprint Interface
	virtual bool SupportedByDefaultBlueprintFactory() const override;
	virtual UClass* GetBlueprintClass()  const override;
	//End of UBlueprintInterface

	public:
	
	/** Graph for adventure asset */
	UPROPERTY()
	class UEdGraph* AdventureGraph;
	
};

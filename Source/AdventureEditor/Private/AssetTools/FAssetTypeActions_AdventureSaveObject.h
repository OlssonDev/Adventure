// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FAssetTypeActions_AdventureBase.h"

class FAssetTypeActions_AdventureSaveObject : public FAssetTypeActions_AdventureBase
{
	public:
	
	FAssetTypeActions_AdventureSaveObject();
	
	// IAssetTypeActions interface
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual const TArray<FText>& GetSubMenus() const override;
	virtual UFactory* GetFactoryForBlueprintType(UBlueprint* InBlueprint) const override;
	// End of IAssetTypeActions interface
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetTypeActions/AssetTypeActions_Blueprint.h"

/**
 * 
 */
class FAssetTypeActions_AdventureBase : public FAssetTypeActions_Blueprint
{
	public:

	FAssetTypeActions_AdventureBase();
	
	// IAssetTypeActions interface
	virtual FColor GetTypeColor() const override;
	virtual uint32 GetCategories() override;
	// End of IAssetTypeActions interface
};

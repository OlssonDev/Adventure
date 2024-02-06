// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureEditorModule.h"
#include "AssetTypeActions_Base.h"
#include "FAssetTypeActions_AdventureBase.h"

class FAssetTypeActions_AdventureAsset : public FAssetTypeActions_AdventureBase
{
	public:

	FAssetTypeActions_AdventureAsset();
	
	// IAssetTypeActions interface
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;
	virtual UFactory* GetFactoryForBlueprintType(UBlueprint* InBlueprint) const override;
	// End of IAssetTypeActions interface
};

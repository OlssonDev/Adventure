// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/BlueprintFactory.h"
#include "AdventureAssetFactory.generated.h"

UCLASS()
class UAdventureAssetFactory : public UBlueprintFactory
{
	GENERATED_UCLASS_BODY()

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual bool ConfigureProperties() override;
	virtual bool CanCreateNew() const override;
	virtual FString GetDefaultNewAssetName() const override;
	virtual FText GetToolTip() const override;
	virtual FText GetDisplayName() const override;
	// End of UFactory interface
	
};

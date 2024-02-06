// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/BlueprintFactory.h"
#include "UObject/Object.h"
#include "AdventureSaveObjectFactory.generated.h"

/**
 * 
 */
UCLASS()
class UAdventureSaveObjectFactory : public UBlueprintFactory
{
	GENERATED_UCLASS_BODY()

	public:

	// UFactory interface
	virtual bool ConfigureProperties() override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext) override;
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	virtual FText GetToolTip() const override;
	virtual FText GetDisplayName() const override;
	//End of implementation
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "FAssetTypeActions_AdventureBase.h"
#include "AdventureEditorModule.h"

FAssetTypeActions_AdventureBase::FAssetTypeActions_AdventureBase()
{
	
}

FColor FAssetTypeActions_AdventureBase::GetTypeColor() const
{
	return FColor::Emerald;
}

uint32 FAssetTypeActions_AdventureBase::GetCategories()
{
	return FModuleManager::LoadModuleChecked<FAdventureEditorModule>("AdventureEditor").AdventureCategory;
}

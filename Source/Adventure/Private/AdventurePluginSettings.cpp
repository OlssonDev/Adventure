// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventurePluginSettings.h"
#include "AdventureAsset.h"
#include "AdventureSaveObject.h"

UAdventurePluginSettings::UAdventurePluginSettings()
{
	DefaultAdventureClass = UAdventureAsset::StaticClass();
	DefaultSaveObjectClass = UAdventureSaveObject::StaticClass();
}	

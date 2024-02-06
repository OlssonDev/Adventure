// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventurePluginSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta = (DisplayName="Adventure Settings"))
class ADVENTURE_API UAdventurePluginSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	UAdventurePluginSettings();

	public:

	//Gets the adventure settings
	static const UAdventurePluginSettings* Get() { return GetDefault<UAdventurePluginSettings>(); }

	//Is the plugin using gameplay tags?
	static bool IsUsingGameplayTags() { return Get()->bUseGameplayTag; }

	public:

	//The default Adventure Asset class when making a new Adventure Asset. You can change this if you want to your project's parent class.
	UPROPERTY(config, EditAnywhere, Category="Adventure", meta = (AllowAbstract="True"))
	TSubclassOf<class UAdventureAsset> DefaultAdventureClass;

	//The default save object the Adventure Component will use to save.
	UPROPERTY(config, EditAnywhere, Category="Adventure")
	TSubclassOf<class UAdventureSaveObject> DefaultSaveObjectClass;

	//If the plugin should use Gameplay Tag as the default argument in Make/Break Objective instead of Name.
	//This will not convert current placed nodes to GameplayTag or vice versa, to avoid data loss.
	//Just make a new Make/Break Objective node and the Argument pin will appear as GameplayTag.
	UPROPERTY(config, EditAnywhere, Category="Adventure Graph", DisplayName = "Use Gameplay Tag As Default", meta=(ConsoleVariable="adventure.UseGameplayTag"))
	uint32 bUseGameplayTag:1;
	
};

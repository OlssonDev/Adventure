// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/AdventureBlueprintLibrary.h"
#include "AdventureAsset.h"
#include "AdventureComponent.h"
#include "AdventureGlobals.h"
#include "FAdventureGraphNodeMinimal.h"
#include "GameplayTagsManager.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

#define LOCTEXT_NAMESPACE "AdventureBlueprintLibrary"

UAdventureComponent* UAdventureBlueprintLibrary::GetAdventureComponent(AActor* ActorToGetFrom)
{
	return UAdventureGlobals::GetAdventureComponentFromActor(ActorToGetFrom, true);
}

UAdventureComponent* UAdventureBlueprintLibrary::GetAdventureComponentFromGameState(UObject* WorldContextObject)
{
	return GetAdventureComponent(WorldContextObject->GetWorld()->GetGameState());;
}

UAdventureComponent* UAdventureBlueprintLibrary::GetAdventureComponentFromPlayerState(APawn* Character)
{
	return GetAdventureComponent(Character->GetPlayerState());
}

UAdventureAsset* UAdventureBlueprintLibrary::FinishSpawningAdventureOnActor(UAdventureAsset* AdventureToStart)
{
	if (IsValid(AdventureToStart))
	{
		AdventureToStart->GetOwningAdventureComponent()->FinishConstructAdventure(AdventureToStart);
	}
	
	return nullptr;
}

UAdventureAsset* UAdventureBlueprintLibrary::BeginSpawningAdventureOnActor(AActor* ActorToUse, TSubclassOf<UAdventureAsset> AdventureToStart)
{
	if (UAdventureComponent* AdventureComponent = GetAdventureComponent(ActorToUse))
	{
		return AdventureComponent->BeginConstructAdventure(AdventureToStart);
	}
	
	return nullptr;
}

void UAdventureBlueprintLibrary::UpdateObjectiveWithName(AActor* AdventureComponentOwner, const FName Argument, int32 Quantity)
{
	if (UAdventureComponent* AdventureComponent = GetAdventureComponent(AdventureComponentOwner))
	{
		AdventureComponent->UpdateObjective(Argument, Quantity);
	}
}

void UAdventureBlueprintLibrary::UpdateObjectiveWithTag(AActor* AdventureComponentOwner, const FGameplayTag Argument, int32 Quantity)
{
	UpdateObjectiveWithName(AdventureComponentOwner, Argument.GetTagName(), Quantity);
}

void UAdventureBlueprintLibrary::FailObjective(AActor* AdventureComponentOwner, const FName Argument)
{
	if (IsValid(AdventureComponentOwner) && !Argument.IsNone())
	{
		if (UAdventureComponent* AdventureComponent = GetAdventureComponent(AdventureComponentOwner))
		{
			AdventureComponent->FailObjective(Argument);
			return;
		}
	}

	check(AdventureComponentOwner)
	check(!Argument.IsNone())
}

void UAdventureBlueprintLibrary::AbandonAdventure(AActor* AdventureComponentOwner, UAdventureAsset* AdventureToAbandon)
{
	if (IsValid(AdventureComponentOwner) && IsValid(AdventureToAbandon))
	{
		if (UAdventureComponent* AdventureComponent = GetAdventureComponent(AdventureComponentOwner))
		{
			AdventureComponent->AbandonAdventure(AdventureToAbandon);
		}
	}
}

void UAdventureBlueprintLibrary::RestartAdventure(AActor* AdventureComponentOwner, UAdventureAsset* AdventureToRestart)
{
	if (IsValid(AdventureComponentOwner) && IsValid(AdventureToRestart))
	{
		if (UAdventureComponent* AdventureComponent = GetAdventureComponent(AdventureComponentOwner))
		{
			AdventureComponent->RestartAdventure(AdventureToRestart);
		}
	}

	check(AdventureComponentOwner)
	check(AdventureToRestart)
}

void UAdventureBlueprintLibrary::AddConditionTagWithName(AActor* AdventureComponentOwner, FName Argument, FGameplayTag NewConditionTag)
{
	if (IsValid(AdventureComponentOwner))
	{
		if (UAdventureComponent* AdventureComponent = GetAdventureComponent(AdventureComponentOwner))
		{
			AdventureComponent->AddConditionTag(Argument, NewConditionTag);
		}
	}
}

void UAdventureBlueprintLibrary::AddConditionTagWithTag(AActor* AdventureComponentOwner, FGameplayTag Argument, FGameplayTag NewConditionTag)
{
	AddConditionTagWithName(AdventureComponentOwner, Argument.GetTagName(), NewConditionTag);
}

FAdventureObjectiveEntry UAdventureBlueprintLibrary::MakeObjectiveWithName(FName Argument, FText Description, int32 Quantity, FGameplayTagContainer OptionalTags, int32 Timer)
{
	FAdventureObjectiveEntry NewObjective;
	NewObjective.Argument = Argument;
	NewObjective.Description = Description;
	NewObjective.Quantity = Quantity;
	NewObjective.OptionalTags = OptionalTags;
	NewObjective.Timer = Timer;
	return NewObjective;
}

FAdventureObjectiveEntry UAdventureBlueprintLibrary::MakeObjectiveWithTag(FGameplayTag Argument, FText Description, int32 Quantity, FGameplayTagContainer OptionalTags, bool IsTimed, int32 Timer)
{
	return MakeObjectiveWithName(Argument.GetTagName(), Description, Quantity, OptionalTags, Timer);
}

void UAdventureBlueprintLibrary::BreakObjectiveWithName(const FAdventureObjectiveEntry& ObjectiveInfo, FName& Argument,FText& Description, int32& Quantity, int32& CurrentProgress, FGameplayTagContainer& OptionalTags, int32& Timer)
{
	Argument = ObjectiveInfo.Argument;
	Description = ObjectiveInfo.Description;
	Quantity = ObjectiveInfo.Quantity;
	CurrentProgress = ObjectiveInfo.CurrentProgress;
	OptionalTags = ObjectiveInfo.OptionalTags;
	Timer = ObjectiveInfo.Timer;
}

void UAdventureBlueprintLibrary::BreakObjectiveWithTag(const FAdventureObjectiveEntry& ObjectiveInfo, FGameplayTag& Argument,FText& Description, int32& Quantity, int32& CurrentProgress, FGameplayTagContainer& OptionalTags, int32& Timer)
{
	FName ArgumentParam;
	BreakObjectiveWithName(ObjectiveInfo, ArgumentParam, Description, Quantity, CurrentProgress, OptionalTags, Timer);
	Argument = FGameplayTag::RequestGameplayTag(ObjectiveInfo.Argument);
	check(Argument.IsValid())
}

#undef LOCTEXT_NAMESPACE

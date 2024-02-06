// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FAdventureGraphNodeMinimal.h"
#include "Adventure/AdventureGameplayTags.h"
#include "AdventureBlueprintLibrary.generated.h"

class UAdventureAsset;

UCLASS()
class ADVENTURE_API UAdventureBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:

	//Get the adventure component from an actor. C++ projects can implement IAdventureInterface::GetAdventureComponent into the Actor for faster getter.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Adventure Component", DisplayName = "Get Adventure Component", meta = (CompactNodeTitle="Adventure Component"))
	static class UAdventureComponent* GetAdventureComponent(AActor* ActorToGetFrom);

	//Gets the adventure component if it's on the game state
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Adventure Component", meta=(WorldContext="WorldContextObject", HidePin="WorldContextObject"))
	static UAdventureComponent* GetAdventureComponentFromGameState(UObject* WorldContextObject);

	//Gets the adventure component if it's on the player state
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Adventure Component")
	static UAdventureComponent* GetAdventureComponentFromPlayerState(APawn* Character);

	//Updates the objectives that's matching the argument.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Adventure | Tasks")
	static void UpdateObjectiveWithName(AActor* AdventureComponentOwner, const FName Argument, int32 Quantity = 1);

	//Updates the objectives that's matching the gameplay tag argument. This is the same as UpdateObjective, but uses a tag.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Adventure | Tasks")
	static void UpdateObjectiveWithTag(AActor* AdventureComponentOwner, const FGameplayTag Argument, int32 Quantity = 1);

	//Fails the objectives that's matching the argument.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Adventure | Tasks")
	static void FailObjective(AActor* AdventureComponentOwner, const FName Argument);

	//Abandon a adventure.
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Adventure | Tasks")
	static void AbandonAdventure(AActor* AdventureComponentOwner, UAdventureAsset* AdventureToAbandon);

	//Restarts a adventure
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Adventure | Tasks")
	static void RestartAdventure(AActor* AdventureComponentOwner, UAdventureAsset* AdventureToRestart);

	//Adds a condition to a objective that matches the argument name
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Adventure | Tasks")
	static void AddConditionTagWithName(AActor* AdventureComponentOwner, FName Argument, FGameplayTag NewConditionTag);

	//Adds a condition to a objective that matches the argument tag
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Adventure | Tasks")
	static void AddConditionTagWithTag(AActor* AdventureComponentOwner, FGameplayTag Argument, FGameplayTag NewConditionTag);

	//Is Objective Entry A equal with B?
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (Keywords="==,Equal", CompactNodeTitle="=="))
	static bool Equal(FAdventureObjectiveEntry A, FAdventureObjectiveEntry B)
	{
		return A == B;
	}

	//Is Objective Entry A not equal to B?
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (Keywords="!=,NotEqual, Not Equal", CompactNodeTitle="!="))
	static bool NotEqual(FAdventureObjectiveEntry A, FAdventureObjectiveEntry B)
	{
		return A != B;
	};

	//Check if objective is completed
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle="Completed"))
	static bool IsCompleted(FAdventureObjectiveEntry ObjectiveInfo)
	{
		return ObjectiveInfo.IsCompleted();
	};

	//Get the argument as Name.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle=Name))
	static FName GetArgumentAsName(FAdventureObjectiveEntry Objective)
	{
		return Objective.GetArgumentAsName();
	};

	//Get the argument as Gameplay Tag, if it's set as a Gameplay Tag, otherwise it will return empty.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle=Tag))
	static FGameplayTag GetArgumentAsGameplayTag(FAdventureObjectiveEntry Objective)
	{
		return Objective.GetArgumentAsGameplayTag();
	};

	//Get Quantity from AdventureObjectiveEntry struct.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle=Quantity))
	static int32 GetQuantity(FAdventureObjectiveEntry Objective)
	{
		return Objective.Quantity;
	};
	
	//Get Description from AdventureObjectiveEntry struct.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle=Description))
	static FText GetDescription(FAdventureObjectiveEntry Objective)
	{
		return Objective.Description;
	}

	//Get Current Progress from AdventureObjectiveEntry struct.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle="Current Progress"))
	static int32 GetCurrentProgress(FAdventureObjectiveEntry Objective)
	{
		return Objective.CurrentProgress;
	}

	//Get Tags from AdventureObjectiveEntry struct.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle=Tags))
	static FGameplayTagContainer GetTags(FAdventureObjectiveEntry Objective)
	{
		return Objective.OptionalTags;
	}

	//Get IsTimed from AdventureObjectiveEntry struct.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle="Is Timed"))
	static bool GetIsTimed(FAdventureObjectiveEntry Objective)
	{
		return Objective.OptionalTags.HasTag(FAdventureGameplayTags::Get().Adventure_Objective_IsTimed);
	}

	//Get Timer from AdventureObjectiveEntry struct.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle=Timer))
	static int32 GetTimer(FAdventureObjectiveEntry Objective)
	{
		return Objective.Timer;
	}

	//Get this objective's condition tags
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle="Condition Tags"))
	static FGameplayTagContainer GetConditionTags(FAdventureObjectiveEntry Objective)
	{
		return Objective.ConditionTags;
	};

	//Check if this objective can be pre-completed.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle="Can Be Pre-Completed"))
	static bool GetCanBePreCompleted(FAdventureObjectiveEntry Objective)
	{
		return Objective.CanBePreCompleted();
	}

	//Get if this objective is optional.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle="Can Be Pre-Completed"))
	static bool GetIsOptional(FAdventureObjectiveEntry Objective)
	{
		return Objective.IsOptional();
	}

	//Has this objective failed?
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle="Has Failed"))
	static bool HasFailed(FAdventureObjectiveEntry Objective)
	{
		return Objective.ConditionTags.HasTag(FAdventureGameplayTags::Get().Adventure_Objective_HasFailed);
	};

	//Objective Unique ID
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Objective Entry", meta = (CompactNodeTitle="Unique ID"))
	static int64 GetObjectiveUniqueID(FAdventureObjectiveEntry Objective)
	{
		return Objective.ObjectiveUniqueID;
	};
	
	//Makes an objective with a localized description
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, BlueprintPure, Category = "Adventure | Objective Entry", meta = (NativeMakeFunc, AdvancedDisplay="2", Normal="0,0,1", ImpactNormal="0,0,1"))
	static FAdventureObjectiveEntry MakeObjectiveWithName(FName Argument, FText Description, int32 Quantity, FGameplayTagContainer OptionalTags, int32 Timer);

	//Makes an objective with a localized description
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, BlueprintPure, Category = "Adventure | Objective Entry", meta = (NativeMakeFunc, AdvancedDisplay="2", Normal="0,0,1", ImpactNormal="0,0,1"))
	static FAdventureObjectiveEntry MakeObjectiveWithTag(FGameplayTag Argument, FText Description, int32 Quantity, FGameplayTagContainer OptionalTags, bool IsTimed, int32 Timer);

	//Breaks the objective entry struct
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, BlueprintPure, Category = "Adventure | Objective Entry", meta = (NativeBreakFunc, AdvancedDisplay="2", Normal="0,0,1", ImpactNormal="0,0,1"), DisplayName = "Break Objective")
	static void BreakObjectiveWithName(const FAdventureObjectiveEntry& ObjectiveInfo, FName& Argument, FText& Description, int32& Quantity, int32& CurrentProgress, FGameplayTagContainer& OptionalTags, int32& Timer);

	//Breaks the objective entry struct
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, BlueprintPure, Category = "Adventure | Objective Entry", meta = (NativeBreakFunc, AdvancedDisplay="2", Normal="0,0,1", ImpactNormal="0,0,1"), DisplayName = "Break Objective")
	static void BreakObjectiveWithTag(const FAdventureObjectiveEntry& ObjectiveInfo, FGameplayTag& Argument, FText& Description, int32& Quantity, int32& CurrentProgress, FGameplayTagContainer& OptionalTags, int32& Timer);
	
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static UAdventureAsset* FinishSpawningAdventureOnActor(UAdventureAsset* AdventureToStart);
	
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	static UAdventureAsset* BeginSpawningAdventureOnActor(AActor* ActorToUse, UPARAM(meta = (AllowAbstract = "false")) TSubclassOf<UAdventureAsset> AdventureToStart);
};

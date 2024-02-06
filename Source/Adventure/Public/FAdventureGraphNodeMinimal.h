#pragma once

#include "GameplayTagContainer.h"
#include "FAdventureGraphNodeMinimal.generated.h"

class UAdventureAsset;

UENUM(BlueprintType)
enum EAdventureState
{
	Completed,
	Failed,
	Ongoing,
	Abandoned
};

USTRUCT(BlueprintType, meta = (HasNativeMake="AdventureBlueprintLibrary.MakeObjectiveWithName", HasNativeBreak="AdventureBlueprintLibrary.BreakObjectiveWithName"))
struct FAdventureObjectiveEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	//The name that will be used to update objectives.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Settings - Objectives", NotReplicated)
	FName Argument;

	#if WITH_EDITORONLY_DATA

	//This is only for editor purposes. You can use GameplayTags to update objectives in the plugin
	//But this is only in editor and in the end it uses the FName instead.
	
	//The Gameplay Tag that will be used to update objectives.
	UPROPERTY(NotReplicated, EditAnywhere, DisplayName = "Argument", Category = "Objective Settings - Objectives")
	FGameplayTag ArgumentTag;

	#endif
	
	//The description of this objective
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Settings - Objectives")
	FText Description;

	//The amount of times we need to complete this action to move on to the next part of the Adventure
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Objective Settings - Objectives", meta = (ClampMin = 1))
	int32 Quantity = 1;

	//How many times this objective has been completed.
	UPROPERTY(BlueprintReadOnly, Category = "Objective Settings - Objectives")
	int32 CurrentProgress = 0;

	//Optional tags for tagging your objectives
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Settings - Objectives")
	FGameplayTagContainer OptionalTags;

	//Tags to change the condition of this objective. Like if it the objective has failed.
	UPROPERTY(BlueprintReadWrite, Category = "Objective Settings - Objectives")
	FGameplayTagContainer ConditionTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective Settings - Objectives")
	int32 Timer = 0;

	UPROPERTY()
	int64 ObjectiveUniqueID;
	
	FAdventureObjectiveEntry()
	{
		OptionalTags.Reset();
		Argument = {};
		Quantity = 1;
	}

	//Checks if this objective has been completed
	bool IsCompleted() const;

	//Resets this objective
	void ResetObjective();

	//Get the argument as FName
	FName GetArgumentAsName();

	//Get the argument as GameplayTag
	FGameplayTag GetArgumentAsGameplayTag();

	//If this objective is optional
	bool IsOptional();

	//If this objective can be pre-completed
	bool CanBePreCompleted();
	
	bool operator == (const FAdventureObjectiveEntry& Other) const
	{
		return Argument == Other.Argument
		&& Quantity == Other.Quantity
		&& CurrentProgress == Other.CurrentProgress
		&& ConditionTags == Other.ConditionTags
		&& OptionalTags == Other.OptionalTags;
	}

	bool operator != (const FAdventureObjectiveEntry& Other) const
	{
		return Argument != Other.Argument
		|| Quantity != Other.Quantity
		|| CurrentProgress != Other.CurrentProgress
		|| ConditionTags != Other.ConditionTags
		|| OptionalTags != Other.OptionalTags;
	}
};

DECLARE_MULTICAST_DELEGATE_OneParam(FObjectiveStepCompleted, FAdventureObjectiveEntry)
DECLARE_MULTICAST_DELEGATE_OneParam(FObjectiveListCompleted, int64)
DECLARE_MULTICAST_DELEGATE_TwoParams(FObjectiveEntryUpdated, FAdventureObjectiveEntry, int32)

USTRUCT(BlueprintType)
struct FAdventureObjectiveList : public FFastArraySerializer
{
	GENERATED_BODY()
	
	//~FFastArraySerializer implementation
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer implementation

	void AddEntry(FAdventureObjectiveEntry NewEntry);
	void RemoveEntryWithArgument(FName Argument);
	void UpdateEntry(FName Argument, int32 Quantity);

	void AddConditionTag(FName Argument, FGameplayTag NewConditionTag);

	bool ObjectiveListCompleted(int64 ObjectiveUniqueID);

	TArray<FAdventureObjectiveEntry> GetEntries();

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FAdventureObjectiveEntry>(Entries, DeltaParms, *this);
	}

	void BroadcastChangeMessage(int32 Index);
	void Append(TArray<FAdventureObjectiveEntry> ObjectiveList);

	private:
	
	// Replicated list of items
	UPROPERTY()
	TArray<FAdventureObjectiveEntry> Entries;

	public:
	
	FObjectiveListCompleted ObjectiveCompleted;
	FObjectiveStepCompleted ObjectiveStepCompleted;
	FObjectiveEntryUpdated ObjectiveConditionTagsUpdated;
	FObjectiveEntryUpdated ObjectiveEntryUpdated;
	
};

template<>
struct TStructOpsTypeTraits<FAdventureObjectiveList> : public TStructOpsTypeTraitsBase2<FAdventureObjectiveList>
{
	enum 
	{
		WithNetDeltaSerializer = true,
   };
};


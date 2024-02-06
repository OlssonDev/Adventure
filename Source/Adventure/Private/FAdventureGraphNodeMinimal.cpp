#include "FAdventureGraphNodeMinimal.h"
#include "Adventure/AdventureGameplayTags.h"
#include "Adventure/AdventureLogChannels.h"

bool FAdventureObjectiveEntry::IsCompleted() const
{
	return CurrentProgress >= Quantity;
}

void FAdventureObjectiveEntry::ResetObjective()
{
	CurrentProgress = 0;
}

FName FAdventureObjectiveEntry::GetArgumentAsName()
{
	return Argument;
}

FGameplayTag FAdventureObjectiveEntry::GetArgumentAsGameplayTag()
{
	return FGameplayTag::RequestGameplayTag(Argument);
}

bool FAdventureObjectiveEntry::IsOptional()
{
	return OptionalTags.HasTag(FAdventureGameplayTags::Get().Adventure_Objective_IsOptional);	
}

bool FAdventureObjectiveEntry::CanBePreCompleted()
{
	return OptionalTags.HasTag(FAdventureGameplayTags::Get().Adventure_Objective_CanBePreCompleted);	
}

void FAdventureObjectiveList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	for (int32 Index : RemovedIndices)
	{
		BroadcastChangeMessage(Index);
	}
}

void FAdventureObjectiveList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		BroadcastChangeMessage(Index);
	}
}

void FAdventureObjectiveList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	for (int32 Index : ChangedIndices)
	{
		BroadcastChangeMessage(Index);
	}
}

void FAdventureObjectiveList::AddEntry(FAdventureObjectiveEntry InNewEntry)
{
	FAdventureObjectiveEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry = InNewEntry;
	MarkItemDirty(NewEntry);
	BroadcastChangeMessage(Entries.Num() - 1);
	UE_LOG(LogAdventure, Log, TEXT("Successfully added an objective with Argument: %s"), *InNewEntry.Argument.ToString())
}

void FAdventureObjectiveList::RemoveEntryWithArgument(FName Argument)
{
	int32 Index = 0;
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FAdventureObjectiveEntry& Entry = *EntryIt;
		if (Entry.Argument == Argument)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
			BroadcastChangeMessage(Index);
			
			return;
		}

		Index++;
	}
}

void FAdventureObjectiveList::UpdateEntry(FName Argument, int32 Quantity)
{
	//Quantity should never be 0.
	ensureAlways(Quantity > 0);
	for (int32 i = 0; i < Entries.Num(); i++ )
	{
		FAdventureObjectiveEntry& Entry = Entries[i];
		if (Entry.Argument == Argument)
		{
			int32 NewQuantity = Entry.CurrentProgress + Quantity;
			Entry.CurrentProgress = FMath::Clamp(NewQuantity, 0, NewQuantity);
			
			ObjectiveStepCompleted.Broadcast(Entry);
			MarkItemDirty(Entry);
			BroadcastChangeMessage(i);
			
			if (ObjectiveListCompleted(Entry.ObjectiveUniqueID))
			{
				ObjectiveCompleted.Broadcast(Entry.ObjectiveUniqueID);
				return;
			}
		}
	}
}

void FAdventureObjectiveList::AddConditionTag(FName Argument, FGameplayTag NewConditionTag)
{
	int32 Index = 0;
	for (FAdventureObjectiveEntry& Entry : Entries)
	{
		if (Entry.Argument == Argument)
		{
			int32 ConditionTagsAmount = Entry.ConditionTags.Num();
			Entry.ConditionTags.AddTag(NewConditionTag);

			//Just mark it as dirty if we actually added a tag
			if (ConditionTagsAmount != Entry.ConditionTags.Num())
			{
				MarkItemDirty(Entry);
				ObjectiveConditionTagsUpdated.Broadcast(Entry, Index);
			}

			return;
		}

		Index++;
	}
}

bool FAdventureObjectiveList::ObjectiveListCompleted(int64 ObjectiveUniqueID)
{
	for (FAdventureObjectiveEntry Entry : Entries)
	{
		if (ObjectiveUniqueID == Entry.ObjectiveUniqueID && Entry.IsCompleted() == false)
		{
			if (Entry.IsOptional() == false)
			{
				return false;
			}
		}
	}

	return true;
}

TArray<FAdventureObjectiveEntry> FAdventureObjectiveList::GetEntries()
{
	return Entries;
}

void FAdventureObjectiveList::BroadcastChangeMessage(int32 Index)
{
#if !UE_SERVER
	ObjectiveEntryUpdated.Broadcast(Entries[Index], Index);
#endif
}

void FAdventureObjectiveList::Append(TArray<FAdventureObjectiveEntry> ObjectiveList)
{
	for (FAdventureObjectiveEntry ObjectiveToAdd : ObjectiveList)
	{
		AddEntry(ObjectiveToAdd);
	}
}

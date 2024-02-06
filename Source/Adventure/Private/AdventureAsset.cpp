// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureAsset.h"
#include "AdventureComponent.h"
#include "AdventureGlobals.h"
#include "Adventure/AdventureGameplayTags.h"
#include "Engine/AssetManager.h"
#include "Net/UnrealNetwork.h"

#define LOCTEXT_NAMESPACE "Adventure"

UAdventureAsset::UAdventureAsset()
{
	AdventureTitle = LOCTEXT("AdventureTitle_DefaultValue", "My New Adventure");
	AdventureDescription = LOCTEXT("AdventureDescription_DefaultValue", "Description of your quest");
}

void UAdventureAsset::OnCompleteAdventure()
{
	SetAdventureState(EAdventureState::Completed);
	K2_OnAdventureCompleted();

	LoadChainedAdventures(FOnChainedAdventuresLoaded::CreateWeakLambda(this, [this](TArray<TSubclassOf<UAdventureAsset>> LoadedAdventures)
	{
		UAdventureComponent* AdventureComponent = GetOwningAdventureComponent();
		check(AdventureComponent)
		
		for (TSubclassOf<UAdventureAsset> AdventureToStart : LoadedAdventures)
		{
			AdventureComponent->StartAdventure(AdventureToStart);
		}
	}));
}

void UAdventureAsset::OnFailAdventure()
{
	SetAdventureState(EAdventureState::Failed);
}

void UAdventureAsset::OnAdvandonAdventure()
{
	SetAdventureState(EAdventureState::Abandoned);
}

void UAdventureAsset::AdventureBeginPlay()
{
	K2_OnAdventureBeginPlay();
}

void UAdventureAsset::InitializeAdventure()
{
	if (!bHasInit)
	{
		bHasInit = true;
		K2_OnAdventureInitialized();
	}
}

void UAdventureAsset::K2_FailAdventure(bool bRestart)
{
	FailAdventure(bRestart);
}

void UAdventureAsset::FailAdventure(bool bRestart)
{
	GetOwningAdventureComponent()->FailAdventure(this, bRestart);
	K2_OnAdventureFailed();
}

void UAdventureAsset::K2_CompleteAdventure()
{
	CompleteAdventure();
}

void UAdventureAsset::CompleteAdventure()
{
	GetOwningAdventureComponent()->CompleteAdventure(this);
}

void UAdventureAsset::K2_AbandonAdventure()
{
	AbandonAdventure();
}

void UAdventureAsset::AbandonAdventure()
{
	GetOwningAdventureComponent()->AbandonAdventure(this);
}

UAdventureComponent* UAdventureAsset::GetOwningAdventureComponent() const
{
	if (AActor* OwningActor = GetOwningActor())
	{
		return UAdventureGlobals::GetAdventureComponentFromActor(OwningActor, true);
	}
	return nullptr;
}

bool UAdventureAsset::HasCompletedCurrentObjectives()
{
	for (FAdventureObjectiveEntry CurrentObjective : ObjectiveList.GetEntries())
	{
		if (!CurrentObjective.IsCompleted())
		{
			return false;
		}
	} 
	return true;
}

void UAdventureAsset::UpdateObjective(FName Argument)
{
	UpdateObjective(Argument, 1);
}

void UAdventureAsset::UpdateObjective(FName Argument, int32 Quantity)
{
	ObjectiveList.UpdateEntry(Argument, Quantity);
}

void UAdventureAsset::FailObjective(FName Argument)
{
	FGameplayTag HasFailedTag = FAdventureGameplayTags::Get().Adventure_Objective_HasFailed;
	ObjectiveList.AddConditionTag(Argument, HasFailedTag);
}

void UAdventureAsset::SetAdventureState(EAdventureState NewAdventureState)
{
	if (NewAdventureState != AdventureState)
	{
		AdventureState = NewAdventureState;
		OnRep_AdventureState();
	}
}

void UAdventureAsset::LoadChainedAdventures(FOnChainedAdventuresLoaded OnChainedAdventuresLoaded)
{
	if (ChainedAdventures.Num())
	{
		TArray<FSoftObjectPath> PathsToLoad;
		for (TSoftClassPtr<UAdventureAsset> SoftClassPtr : ChainedAdventures)
		{
			PathsToLoad.AddUnique(SoftClassPtr.ToSoftObjectPath());
		}
	
		UAssetManager::GetStreamableManager().RequestAsyncLoad(PathsToLoad, FStreamableDelegate::CreateWeakLambda(this, [this, OnChainedAdventuresLoaded]
		{
			TArray<TSubclassOf<UAdventureAsset>> LoadedAdventures;
			for (TSoftClassPtr<UAdventureAsset> SoftClassPtr : ChainedAdventures)
			{
				LoadedAdventures.Add(SoftClassPtr.Get());
			}

			OnChainedAdventuresLoaded.ExecuteIfBound(LoadedAdventures);
		}));
	}
}

void UAdventureAsset::OnRep_AdventureState()
{
	OnAdventureStateModified.Broadcast(AdventureState);
	K2_OnNewAdventureState(AdventureState);
}

void UAdventureAsset::AddConditionTagWithName(FName Argument, FGameplayTag NewConditionTag)
{
	ObjectiveList.AddConditionTag(Argument, NewConditionTag);
}

void UAdventureAsset::AddConditionTagWithTag(FGameplayTag Argument, FGameplayTag NewConditionTag)
{
	AddConditionTagWithName(Argument.GetTagName(), NewConditionTag);
}

bool UAdventureAsset::IsLoadingSaveFile() const
{
	return GetOwningAdventureComponent()->IsLoadingSaveFile;
}

AActor* UAdventureAsset::GetOwningActor() const
{
	return GetTypedOuter<AActor>();
}

APawn* UAdventureAsset::GetOwningPawn() const
{
	return GetTypedOuter<APawn>();
}

void UAdventureAsset::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, AdventureState);
	DOREPLIFETIME(ThisClass, ObjectiveList);
	if (const UBlueprintGeneratedClass* BPClass = Cast<UBlueprintGeneratedClass>(GetClass()))
	{
		BPClass->GetLifetimeBlueprintReplicationList(OutLifetimeProps);
	}
}

int32 UAdventureAsset::GetFunctionCallspace(UFunction* Function, FFrame* Stack)
{
	if (HasAnyFlags(RF_ClassDefaultObject) || !IsSupportedForNetworking())
	{
		return GEngine->GetGlobalFunctionCallspace(Function, this, Stack);
	}
	check(GetOuter() != nullptr);
	return GetOuter()->GetFunctionCallspace(Function, Stack);
}

bool UAdventureAsset::CallRemoteFunction(UFunction* Function, void* Parms, FOutParmRec* OutParms, FFrame* Stack)
{
	check(!HasAnyFlags(RF_ClassDefaultObject));
	AActor* Owner = GetOwningActor();
	UNetDriver* NetDriver = Owner->GetNetDriver();
	if (NetDriver)
	{
		NetDriver->ProcessRemoteFunction(Owner, Function, Parms, OutParms, Stack, this);
		return true;
	}
	return false;
}

FPrimaryAssetId UAdventureAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("AdventureAsset", GetFName());
}

void UAdventureAsset::AddNewObjectiveList(TArray<FAdventureObjectiveEntry> NewObjectives, int64& NewObjectiveUniqueID)
{
	NewObjectiveUniqueID = GetNextObjectiveListUniqueID();
	for (FAdventureObjectiveEntry NewObjective : NewObjectives)
	{
		AddObjectiveToActiveList(NewObjectiveUniqueID, NewObjective);
	}
}

void UAdventureAsset::AddObjectiveToActiveList(int64 ObjectiveListID, FAdventureObjectiveEntry NewObjective)
{
	NewObjective.ObjectiveUniqueID = ObjectiveListID;
	ObjectiveList.AddEntry(NewObjective);

	if (const auto* FoundEntry = GetOwningAdventureComponent()->ObjectivesList.Find(NewObjective.Argument))
	{
		ObjectiveList.UpdateEntry(NewObjective.Argument, FMath::Clamp(NewObjective.CurrentProgress + *FoundEntry, 0, NewObjective.Quantity));
	}
}

TArray<FAdventureObjectiveEntry> UAdventureAsset::MakeObjectiveArray(TArray<FAdventureObjectiveEntry> ObjectiveArray, FAdventureObjectiveEntry ObjectiveEntry)
{
	if (ObjectiveArray.Num())
	{
		ObjectiveArray.Add(ObjectiveEntry);
		return ObjectiveArray;
	}
	
	TArray<FAdventureObjectiveEntry> NewList;
	NewList.Add(ObjectiveEntry);
	return NewList;
}

UWorld* UAdventureAsset::GetWorld() const
{
	if (GetOuter() == nullptr)
	{
		return nullptr;
	}
		
	if (Cast<UPackage>(GetOuter()) != nullptr)
	{
		return Cast<UWorld>(GetOuter()->GetOuter());
	}
		
	return GetOwningActor()->GetWorld();
}

#undef LOCTEXT_NAMESPACE


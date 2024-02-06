// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureComponent.h"
#include "AdventureAsset.h"
#include "AdventureMacros.h"
#include "AdventurePluginSettings.h"
#include "AdventureSaveObject.h"
#include "Adventure/AdventureGameplayTags.h"
#include "Adventure/AdventureLogChannels.h"
#include "Engine/ActorChannel.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

#define LOCTEXT_NAMESPACE "AdventureComponent"

UAdventureComponent::UAdventureComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

UAdventureAsset* UAdventureComponent::BeginConstructAdventure(TSubclassOf<UAdventureAsset> AdventureToStart)
{
	if (CanStartAdventure(AdventureToStart))
	{
		return NewObject<UAdventureAsset>(this, AdventureToStart);
	}

	return nullptr;
}

void UAdventureComponent::FinishConstructAdventure(UAdventureAsset* AdventureToStart)
{
	if (IsValid(AdventureToStart))
	{
		AdventureToStart->AdventureBeginPlay();
		
		ActiveAdventures.Add(AdventureToStart);
		OnRep_ActiveAdventures();
	}
	else
	{
		ADVENTURE_ERROR("Can't finish constructing Adventure. AdventureToStart was invalid.")
	}
}

UAdventureAsset* UAdventureComponent::StartAdventure(TSubclassOf<UAdventureAsset> AdventureToStart)
{
	if (UAdventureAsset* NewAdventure = BeginConstructAdventure(AdventureToStart))
	{
		FinishConstructAdventure(NewAdventure);
	}

	return nullptr;
}

bool UAdventureComponent::CanStartAdventure(TSubclassOf<UAdventureAsset> AdventureToStart)
{
	if (IsValid(AdventureToStart))
	{
		const FString ErrorMessage = FString::Printf(TEXT("Failed to start %s"), *AdventureToStart.GetDefaultObject()->GetClass()->GetName());

		if (!AdventureToStart.GetDefaultObject()->bEnabled)
		{
			UE_LOG(LogAdventure, Log, TEXT("%s. Reason: Adventure is not enabled"), *ErrorMessage)
			return false;
		}

		if (CheckIsAdventureActiveWithClass(AdventureToStart))
		{
			UE_LOG(LogAdventure, Log, TEXT("%s. Reason: Adventure is already active"), *ErrorMessage)
			return false;
		}

		if (CheckIsAdventureCompletedWithClass(AdventureToStart))
		{
			UE_LOG(LogAdventure, Log, TEXT("%s. Reason: Adventure is already completed."), *ErrorMessage)
			return false;
		}

		if (!CanCarryMoreAdventures())
		{
			UE_LOG(LogAdventure, Log, TEXT("%s. Reason: Can't carry more Adventures."), *ErrorMessage)
			return false;
		}
	}
	else
	{
		ADVENTURE_INFO(FString::Printf(TEXT("Can't start Adventure. Reason: Adventure class is not valid.")))
		return false;
	}
	
	
	return true;
}

TArray<UAdventureAsset*> UAdventureComponent::GetAdventureList()
{
	return ActiveAdventures;
}

bool UAdventureComponent::CheckIsAdventureActiveWithClass(TSubclassOf<UAdventureAsset> AdventureToCheck)
{
	return IsValid(GetAdventure(AdventureToCheck));
}

bool UAdventureComponent::CheckIsAdventureActiveWithSoftClass(TSoftClassPtr<UAdventureAsset> AdventureToCheck)
{
	if (AdventureToCheck.IsValid())
	{
		return IsValid(GetAdventure(AdventureToCheck.Get()));
	}

	return false;
}

bool UAdventureComponent::CheckIsAdventureActiveWithAssetID(FPrimaryAssetId AssetIDToCheck)
{
	for (UAdventureAsset* ActiveAdventure : ActiveAdventures)
	{
		if (IsValid(ActiveAdventure) && ActiveAdventure->GetPrimaryAssetId() == AssetIDToCheck)
		{
			return true;
		}
	}

	return false;
}

void UAdventureComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UAdventureComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ThisClass, ActiveAdventures, COND_OwnerOnly);
}

bool UAdventureComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool bWroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	bWroteSomething |= Channel->ReplicateSubobjectList(ActiveAdventures, *Bunch, *RepFlags);
	return bWroteSomething;
}

bool UAdventureComponent::CheckIsAdventureCompletedWithClass(TSubclassOf<UAdventureAsset> AdventureToCheck)
{
	return CompletedAdventures.Contains(AdventureToCheck.GetDefaultObject()->GetPrimaryAssetId());
}

bool UAdventureComponent::CheckIsAdventureCompletedWithAssetID(FPrimaryAssetId AssetIDToCheck)
{
	return CompletedAdventures.Contains(AssetIDToCheck);
}

UAdventureAsset* UAdventureComponent::GetAdventure(TSubclassOf<UAdventureAsset> InAdventureToGet)
{
	if (IsValid(InAdventureToGet))
	{
		for (const auto Adventure : ActiveAdventures)
		{
			if (Adventure->GetClass() == InAdventureToGet) return Adventure;
		}
	}
	
	return nullptr;
}

UAdventureSaveObject* UAdventureComponent::SaveAdventureComponent(FString SlotName)
{
	if (!SlotName.IsEmpty())
	{
		const UAdventurePluginSettings* PluginSettings = GetDefault<UAdventurePluginSettings>();
		TSubclassOf<UAdventureSaveObject> SaveObjectClass = PluginSettings->DefaultSaveObjectClass;
		
		if (!IsValid(SaveObjectClass))
		{
			SaveObjectClass = UAdventureSaveObject::StaticClass();
			FAdventureLogMessages::Error("Adventure Plugin Settings has an invalid DefaultSaveObjectClass class. Using default AdventureSaveObject instead.");
		}
		
		SavingAdventureComponent.Broadcast();
		UAdventureSaveObject* SaveGameInstance = NewObject<UAdventureSaveObject>(GetTransientPackage(), SaveObjectClass);
		SaveGameInstance->SlotName = SlotName;

		//Save the data inside the Adventure component
		FMemoryWriter MemWriter_AdventureComponent(SaveGameInstance->AdventureComponentSaveData);
		FObjectAndNameAsStringProxyArchive Ar_AdventureComponent(MemWriter_AdventureComponent, true);
		Ar_AdventureComponent.ArIsSaveGame = true;
		Serialize(Ar_AdventureComponent);

		//Save the data for all active adventures
		for (UAdventureAsset* ActiveAdventure : ActiveAdventures)
		{
			//Begin with saving the actual Adventure asset and its SaveGame variables
			FSavedActiveAdventure SavedActiveAdventure;
			SavedActiveAdventure.SavedActiveAdventure = ActiveAdventure->GetClass();
		
			FMemoryWriter MemWriter_ActiveAdventure(SavedActiveAdventure.SavedActiveAdventureData);
		
			FObjectAndNameAsStringProxyArchive Ar_ActiveAdventure(MemWriter_ActiveAdventure, true);
			Ar_ActiveAdventure.ArIsSaveGame = true;
			ActiveAdventure->Serialize(Ar_ActiveAdventure);
			SaveGameInstance->SavedActiveAdventures.Add(SavedActiveAdventure);
		}

		SaveGameInstance->SaveToDisk();
		FinishedSavingAdventureComponent.Broadcast();
		return SaveGameInstance;
	}
	else
	{
		ensureAlways(SlotName.IsEmpty());
		FAdventureLogMessages::Error("Can't save Adventure Component. Reason: The slot name is empty.");
	}

	return nullptr;
}

UAdventureSaveObject* UAdventureComponent::LoadAdventureComponent(FString SlotName)
{
	if (!SlotName.IsEmpty())
	{
		if (UAdventureSaveObject* AdventureSaveObject = Cast<class UAdventureSaveObject>(UGameplayStatics::LoadGameFromSlot(SlotName, 0)))
		{
			SendSaveFileToServerAndLoad(AdventureSaveObject->AdventureComponentSaveData, AdventureSaveObject->SavedActiveAdventures);
			return AdventureSaveObject;
		}
		
		ADVENTURE_WARNING(FString::Printf(TEXT("Failed to load save file. Reason: There is no save file with the name %s"), *SlotName));
		
	}
	else
	{
		ADVENTURE_WARNING("Failed to load save file. Reason: Slot name is empty.");
	}
	
	return nullptr;
}

void UAdventureComponent::SendSaveFileToServerAndLoad_Implementation(const TArray<uint8>& AdventureComponentSaveData, const TArray<FSavedActiveAdventure>& SavedActiveAdventures)
{
	if (AdventureComponentSaveData.Num() && SavedActiveAdventures.Num())
	{
		IsLoadingSaveFile = true;
		LoadingAdventureComponent.Broadcast();
		
		//Save Data inside the component
		FMemoryReader MemReader_AdventureComponent(AdventureComponentSaveData);
		FObjectAndNameAsStringProxyArchive Ar_AdventureComponent(MemReader_AdventureComponent, true);
		Ar_AdventureComponent.ArIsSaveGame = true;
		Serialize(Ar_AdventureComponent);

		for (auto SavedActiveAdventure : SavedActiveAdventures)
		{
			FMemoryReader MemReader_ActiveAdventure(SavedActiveAdventure.SavedActiveAdventureData);
			FObjectAndNameAsStringProxyArchive Ar_ActiveAdventure(MemReader_ActiveAdventure, true);
			Ar_ActiveAdventure.ArIsSaveGame = true;
			
			UAdventureAsset* NewAdventure = NewObject<UAdventureAsset>(this, SavedActiveAdventure.SavedActiveAdventure);
			NewAdventure->Serialize(Ar_ActiveAdventure);
			FinishConstructAdventure(NewAdventure);
		}

		IsLoadingSaveFile = false;
		FinishedLoadingAdventureComponent.Broadcast();
	}
}

int32 UAdventureComponent::GetAmountOfActiveAdventures()
{
	return ActiveAdventures.Num();
}

void UAdventureComponent::UpdateObjective(const FName Argument)
{
	UpdateObjective(Argument, 1);
}

void UAdventureComponent::UpdateObjective(const FName Argument, int32 Quantity)
{
	if (!Argument.IsNone())
	{
		if (Quantity == 0)
		{
			ADVENTURE_INFO("Quantity can't be set to 0. Defaulting back to 1.")
			Quantity = 1;
		}
		
		if (const auto* Key = ObjectivesList.Find(Argument))
		{
			ObjectivesList.Add(Argument, *Key + Quantity);
		}
		else
		{
			ObjectivesList.Add(Argument, Quantity);
		}

		//Cache the old active adventures as this array can grow as we update the objectives.
		TArray<UAdventureAsset*> CurrentActiveAdventures { ActiveAdventures };
		for (const auto Adventure : CurrentActiveAdventures)
		{
			//Only update if the adventure is ongoing.
			if (Adventure->AdventureState == EAdventureState::Ongoing)
			{
				Adventure->UpdateObjective(Argument, Quantity);
			}
		}
	}
	else
	{
		ADVENTURE_WARNING("Failed to update objective. Reason: Argument was empty.")
	}
}

void UAdventureComponent::UpdateObjective(const FGameplayTag Argument)
{
	UpdateObjective(Argument.GetTagName());
}

void UAdventureComponent::UpdateObjective(const FGameplayTag Argument, int32 Quantity)
{
	UpdateObjective(Argument.GetTagName(), Quantity);
}

void UAdventureComponent::AddConditionTag(FName Argument, FGameplayTag NewConditionTag)
{
	if (!Argument.IsNone())
	{
		TArray<UAdventureAsset*> CachedActiveAdventures {ActiveAdventures};
		for (const auto Adventure : CachedActiveAdventures)
		{
			Adventure->AddConditionTagWithName(Argument, NewConditionTag);
		}
	}
}

void UAdventureComponent::AddConditionTag(FGameplayTag Argument, FGameplayTag NewConditionTag)
{
	AddConditionTag(Argument.GetTagName(), NewConditionTag);
}

void UAdventureComponent::FailObjective(const FName Argument)
{
	TArray<UAdventureAsset*> CachedActiveAdventures {ActiveAdventures};

	for (const auto Adventure : CachedActiveAdventures)
	{
		Adventure->FailObjective(Argument);
	}
}

void UAdventureComponent::K2_CompleteAdventure(UAdventureAsset* AdventureToComplete)
{
	CompleteAdventure(AdventureToComplete);
}

void UAdventureComponent::K2_FailAdventure(UAdventureAsset* AdventureToFail, bool Restart)
{
	FailAdventure(AdventureToFail, Restart);
}

void UAdventureComponent::K2_AbandonAdventure(UAdventureAsset* AdventureToAbandon)
{
	AbandonAdventure(AdventureToAbandon);
}

void UAdventureComponent::K2_RestartAdventure(UAdventureAsset* AdventureToRestart)
{
	RestartAdventure(AdventureToRestart);
}

void UAdventureComponent::RestartAdventure(UAdventureAsset* AdventureToRestart)
{
	if (IsValid(AdventureToRestart))
	{
		ActiveAdventures.Remove(AdventureToRestart);
		StartAdventure(AdventureToRestart->GetClass());
	}
}

void UAdventureComponent::CompleteAdventure(UAdventureAsset* AdventureToComplete)
{
	if (IsValid(AdventureToComplete))
	{
		AdventureToComplete->OnCompleteAdventure();
		if (!AdventureToComplete->AdventureTags.HasTag(FAdventureGameplayTags::Get().Adventure_NoAutoComplete))
		{
			AddAdventureToCompletedList(AdventureToComplete);
		}
	}
}

void UAdventureComponent::FailAdventure(UAdventureAsset* AdventureToFail, bool Restart)
{
	if (IsValid(AdventureToFail))
	{
		AdventureToFail->OnFailAdventure();
		
		if (Restart)
		{
			RestartAdventure(AdventureToFail);
		}
		else
		{
			ActiveAdventures.Remove(AdventureToFail);
		}

		#if !UE_SERVER
		OnRep_ActiveAdventures();
		#endif
	}
}

void UAdventureComponent::AbandonAdventure(UAdventureAsset* AdventureToAbandon)
{
	if (IsValid(AdventureToAbandon))
	{
		AdventureToAbandon->OnAdvandonAdventure();
	}
}

bool UAdventureComponent::RemoveAdventureWithClass(TSubclassOf<UAdventureAsset> AdventureToRemove)
{
	if (AdventureToRemove)
	{
		int32 Index = 0;
		for (UAdventureAsset* ActiveAdventure : ActiveAdventures)
		{
			if (ActiveAdventure->GetClass() == AdventureToRemove)
			{
				ActiveAdventures.RemoveAt(Index);
				OnRep_ActiveAdventures();
				return true;
			}

			Index++;
		}
	}
	return false;
}

bool UAdventureComponent::RemoveAdventureWithReference(UAdventureAsset* AdventureToRemove)
{
	if (IsValid(AdventureToRemove))
	{
		return RemoveAdventureWithClass(AdventureToRemove->GetClass());
	}
	return false;
}

void UAdventureComponent::RemoveAllActiveAdventures()
{
	ActiveAdventures.Empty();
	OnRep_ActiveAdventures();
}

void UAdventureComponent::AddAdventureToCompletedList(UAdventureAsset* AdventureToAdd)
{
	if (IsValid(AdventureToAdd) && AdventureToAdd->AdventureState == EAdventureState::Completed)
	{
		CompletedAdventures.Add(AdventureToAdd->GetClass()->GetPrimaryAssetId());
	}
}

void UAdventureComponent::OnRep_ActiveAdventures()
{
	ActiveAdventuresModified.Broadcast(ActiveAdventures);

	for (const auto ActiveAdventure : ActiveAdventures)
	{
		if (IsValid(ActiveAdventure))
		{
			ActiveAdventure->InitializeAdventure();
		}
	}
}

#undef LOCTEXT_NAMESPACE
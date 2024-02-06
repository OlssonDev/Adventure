// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdventureAsset.h"
#include "Components/ActorComponent.h"
#include "AdventureSaveObject.h"
#include "AdventureComponent.generated.h"

UCLASS(ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent), HideCategories=(Collsion, Sockets, Cooking), DisplayName = "Adventure Component")
class ADVENTURE_API UAdventureComponent : public UActorComponent
{
	GENERATED_BODY()

	public:

	UAdventureComponent();

	protected:

	//UActorComponent interface implementation
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//End of implementation
	
	//UObject interface implementation
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//End of implementation

	public:

	//Starts a Adventure on this component
	virtual UAdventureAsset* StartAdventure(TSubclassOf<UAdventureAsset> AdventureToStart);
	
	//Constructs the Adventure
	virtual UAdventureAsset* BeginConstructAdventure(TSubclassOf<UAdventureAsset> AdventureToStart);
	
	//Finish constructing the Adventure.
	virtual void FinishConstructAdventure(UAdventureAsset* AdventureToStart);

	//Check if a Adventure can start
	virtual bool CanStartAdventure(TSubclassOf<UAdventureAsset> AdventureToStart);

	public:

	//Gets all adventures that have been registered.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure Component | Adventure", meta = (CompactNodeTitle="List"))
	TArray<UAdventureAsset*> GetAdventureList();

	//Check if an adventure has been registered and is currently active with class param.
	UFUNCTION(BlueprintCallable, Category = "Adventure Component | Adventure", meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CheckIsAdventureActiveWithClass(TSubclassOf<UAdventureAsset> AdventureToCheck);

	//Check if an adventure has been registered and is currently active with soft class param.
	UFUNCTION(BlueprintCallable, Category = "Adventure Component | Adventure", meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CheckIsAdventureActiveWithSoftClass(TSoftClassPtr<UAdventureAsset> AdventureToCheck);

	//Check if an adventure has been registered and is currently active with Asset ID param.
	UFUNCTION(BlueprintCallable, Category = "Adventure Component | Adventure", meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CheckIsAdventureActiveWithAssetID(FPrimaryAssetId AssetIDToCheck);

	//Check if the adventure is already completed with class parameter
	UFUNCTION(BlueprintCallable, Category = "Adventure Component | Adventure", meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CheckIsAdventureCompletedWithClass(TSubclassOf<UAdventureAsset> AdventureToCheck);

	//Check if the adventure is already completed with asset ID
	UFUNCTION(BlueprintCallable, Category = "Adventure Component | Adventure", meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CheckIsAdventureCompletedWithAssetID(FPrimaryAssetId AssetIDToCheck);

	//Get the a reference to a registered Adventure. The Adventure must have been started to be able to get it.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure Component | Adventure")
	UAdventureAsset* GetAdventure(TSubclassOf<UAdventureAsset> AdventureToGet);

	//Saves the adventure component and the Active Adventures and its variables that are set to SaveGame.
	UFUNCTION(BlueprintCallable, Category = "Adventure Component | Saving And Loading")
	UAdventureSaveObject* SaveAdventureComponent(FString SlotName);
	
	//Loads the adventure component and the Active Adventures and its variables that are set to SaveGame.
	UFUNCTION(BlueprintCallable, Category = "Adventure Component | Saving And Loading")
	UAdventureSaveObject* LoadAdventureComponent(FString SlotName);

	//Send the server file to the server and let the server apply the save file
	UFUNCTION(Server, Reliable)
	void SendSaveFileToServerAndLoad(const TArray<uint8>& AdventureComponentSaveData, const TArray<FSavedActiveAdventure>& SavedActiveAdventures);
	
	//Get the amount of active adventures
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure Component")
	int32 GetAmountOfActiveAdventures();

	//Check if this Adventure Component can carry more Adventures or the list is full.
	UFUNCTION(BlueprintCallable, Category = "Adventure Component", meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool CanCarryMoreAdventures() const
	{
		return ActiveAdventures.Num() >= MaxAdventureCount || !HasMaxAdventureCount;
	}

	//Updates an objective using a FName Argument with Quantity 1.
	virtual void UpdateObjective(const FName Argument);
	
	//Updates an objective using a FName Argument and Quantity
	virtual void UpdateObjective(const FName Argument, int32 Quantity);

	//Updates an objective using a GameplayTag Argument with Quantity 1.
	virtual void UpdateObjective(const FGameplayTag Argument);

	//Updates an objective using a GameplayTag Argument and Quantity
	virtual void UpdateObjective(const FGameplayTag Argument, int32 Quantity);

	//Fails an objective with the matching argument.
	virtual void FailObjective(const FName Argument);

	//Adds a condition tag based on a Argument that's a GameplayTag.
	virtual void AddConditionTag(FGameplayTag Argument, FGameplayTag NewConditionTag);

	//Adds a condition tag based on a Argument that's a FName.
	virtual void AddConditionTag(FName Argument, FGameplayTag NewConditionTag);
	
	//Complete the adventure
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	void K2_CompleteAdventure(UAdventureAsset* AdventureToComplete);
	void CompleteAdventure(UAdventureAsset* AdventureToComplete);
	
	//Fail the adventure
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	void K2_FailAdventure(UAdventureAsset* AdventureToFail, bool Restart);
	void FailAdventure(UAdventureAsset* AdventureToFail, bool Restart);

	//Abandons a adventure
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	void K2_AbandonAdventure(UAdventureAsset* AdventureToAbandon);
	void AbandonAdventure(UAdventureAsset* AdventureToAbandon);

	//Restarts a adventure
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
	void K2_RestartAdventure(UAdventureAsset* AdventureToRestart);
	void RestartAdventure(UAdventureAsset* AdventureToRestart);

	//Removes a an Adventure
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Adventure Component")
	bool RemoveAdventureWithClass(TSubclassOf<UAdventureAsset> AdventureToRemove);

	//Remove the adventure from active adventures
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Adventure Component")
	bool RemoveAdventureWithReference(UAdventureAsset* AdventureToRemove);

	//Remove all active adventures for this component
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Adventure Component")
	void RemoveAllActiveAdventures();

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Adventure Component")
	void AddAdventureToCompletedList(UAdventureAsset* AdventureToAdd);

	protected:

	UFUNCTION()
	virtual void OnRep_ActiveAdventures();

	public:

	//All active Adventures. Since the Adventure ptr is just a duplicate of a class, it doesn't have the class.
	UPROPERTY(ReplicatedUsing=OnRep_ActiveAdventures, Transient)
	TArray<UAdventureAsset*> ActiveAdventures;

	//All completed adventures saved as their Asset ID.
	UPROPERTY(SaveGame, BlueprintReadOnly)
	TArray<FPrimaryAssetId> CompletedAdventures;

	//All completed objectives. Can only be read on server.
	UPROPERTY(BlueprintReadOnly, SaveGame)
	TMap<FName, int32> ObjectivesList;

	//If this component has a Max Adventure Count. The player can only have X Adventures active.
	UPROPERTY()
	bool HasMaxAdventureCount;

	//How many adventures this component can carry
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Adventure Component - Size", meta = (EditCondition = "HasMaxAdventureCount"))
	int32 MaxAdventureCount = 0;

	//Is true if Adventure Component is currently loading a save file.
	UPROPERTY(BlueprintReadOnly, Category = "Adventure Component - Save/Load")
	bool IsLoadingSaveFile = false;

	public:

	//Broadcasts when a new adventure has been started.
	DECLARE_MULTICAST_DELEGATE_OneParam(FActiveAdventuresModified, const TArray<UAdventureAsset*>);
	FActiveAdventuresModified ActiveAdventuresModified;

	//Fires when a fail node has been fired.
	DECLARE_MULTICAST_DELEGATE_OneParam(FAdventureFailed, UAdventureAsset*);
	FAdventureFailed OnAdventureFailed;

	//Broadcasts when the adventure component starts saving
	FSimpleMulticastDelegate SavingAdventureComponent;

	//Broadcasts when the adventure component has finished saving
	FSimpleMulticastDelegate FinishedSavingAdventureComponent;

	//Broadcasts when the adventure component starts loading
	FSimpleMulticastDelegate LoadingAdventureComponent;

	//Broadcasts when the adventure component has finished loading
	FSimpleMulticastDelegate FinishedLoadingAdventureComponent;
	
};

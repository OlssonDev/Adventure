// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FAdventureGraphNodeMinimal.h"
#include "AdventureAsset.generated.h"

class UAdventureComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FAdventureStateModified, TEnumAsByte<EAdventureState>);
DECLARE_DELEGATE_OneParam(FOnChainedAdventuresLoaded, TArray<TSubclassOf<UAdventureAsset>>);

//The base class for all Adventures.
UCLASS(Blueprintable, BlueprintType, Abstract)
class ADVENTURE_API UAdventureAsset : public UObject
{
	GENERATED_BODY()

	friend UAdventureComponent;
	
	public:

	UAdventureAsset();

	//UObject interface implementation
	virtual UWorld* GetWorld() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual int32 GetFunctionCallspace(UFunction* Function, FFrame* Stack) override;
	virtual bool CallRemoteFunction(UFunction* Function, void* Parms, struct FOutParmRec* OutParms, FFrame* Stack) override;
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	//End of implementation
	
	virtual void AdventureBeginPlay();
	virtual void InitializeAdventure();

	//Fails the adventure.
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Adventure | State", meta = (Keywords = "Fail, Fail Adventure, Failed"), DisplayName = "Fail Adventure")
	virtual void K2_FailAdventure(bool bRestart);
	virtual void FailAdventure(bool bRestart);
	virtual void OnFailAdventure();
	
	//Completes the adventure.
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Adventure | State", meta = (Keywords = "Complete, Complete Adventure, Succeed"), DisplayName = "Complete Adventure")
	virtual void K2_CompleteAdventure();
	virtual void CompleteAdventure();
	virtual void OnCompleteAdventure();
	
	//Completes the adventure.
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Adventure | State", meta = (Keywords = "Abandon, Forget, Abandon Adventure"), DisplayName = "Abandon Adventure")
	virtual void K2_AbandonAdventure();
	virtual void AbandonAdventure();
	virtual void OnAdvandonAdventure();
	
	virtual void UpdateObjective(FName Argument);
	virtual void UpdateObjective(FName Argument, int32 Quantity);
	
	virtual void FailObjective(FName Argument);

	virtual void SetAdventureState(EAdventureState NewAdventureState);

	public:

	//Get the actor that owns this adventure
	UFUNCTION(BlueprintPure, Category = "Adventure Asset")
	AActor* GetOwningActor() const;

	//Check if the owner has authority
	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Adventure Asset", meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool HasAuthority() const
	{
		return CastChecked<AActor>(GetOwningActor())->HasAuthority();
	}

	//If a pawn is owning this adventure, it will return the pawn.
	UFUNCTION(BlueprintPure, Category = "Adventure Asset")
	APawn* GetOwningPawn() const;

	//Get the Adventure Component that owns/started this Adventure.
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Component", meta = (Keywords = "Adventure Component, Get Adventure Component"))
	UAdventureComponent* GetOwningAdventureComponent() const;

	//Checks if all the current objectives has been completed
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Adventure | Component", meta = (Keywords = "Has Finished All ObjectiveList, ObjectiveList, Finished ObjectiveList"))
	bool HasCompletedCurrentObjectives();

	//Adds an Adventure to the chained adventures. Returns true if it was added.
	UFUNCTION(BlueprintCallable, Category = "Adventure | Chain")
	bool AddAdventureToChain(TSoftClassPtr<UAdventureAsset> AdventureToAdd)
	{
		return ChainedAdventures.AddUnique(AdventureToAdd) != -1;
	};

	//Removes an Adventure from the chained adventures. Returns true if anything was removed.
	UFUNCTION(BlueprintCallable, Category = "Adventure | Chain")
	bool RemoveAdventureFromChain(TSoftClassPtr<UAdventureAsset> AdventureToAdd)
	{
		return ChainedAdventures.Remove(AdventureToAdd) > 0;
	};

	UFUNCTION(BlueprintCallable, Category = "Adventure | Objectives")
	TArray<FAdventureObjectiveEntry> GetAllObjectives()
	{
		return ObjectiveList.GetEntries();
	};

	//Loads the chained adventures
	void LoadChainedAdventures(FOnChainedAdventuresLoaded OnChainedAdventuresLoaded);

	protected:

	//OnRep for AdventureState.
	UFUNCTION()
	virtual void OnRep_AdventureState();
	
	public:

	//Executes when the Adventure has run its BeginPlay on the server. This runs on all clients, and on server.
	UFUNCTION(BlueprintImplementableEvent, Category = "Adventure - Events", DisplayName = "On Adventure Initialized")
	void K2_OnAdventureInitialized();

	//Adventure will start from this node on the Server.
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, DisplayName = "Adventure Begin Play")
	void K2_OnAdventureBeginPlay();

	//Executes when the Adventure has been completed.
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, DisplayName = "On Adventure Completed", meta = (Keywords = "On Completed, OnCompleted, On Adventure Completed, OnAdventureCompleted"))
	void K2_OnAdventureCompleted();

	//Executes when the Adventure has failed.
	UFUNCTION(BlueprintImplementableEvent, BlueprintAuthorityOnly, DisplayName = "On Adventure Failed", meta = (Keywords = "On Failed, OnFailed, On Adventure Failed, OnAdventureFailed"))
	void K2_OnAdventureFailed();

	//Executes when the Adventure has changed states. Executes on all clients, and on server.
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "On New Adventure State", meta = (Keywords = "On New State, New State, On New Adventure State, OnNewAdventureState"))
	void K2_OnNewAdventureState(EAdventureState NewState);

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Adventure Component - Condition")
	void AddConditionTagWithName(FName Argument, FGameplayTag NewConditionTag);

	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Adventure Component - Condition")
	void AddConditionTagWithTag(FGameplayTag Argument, FGameplayTag NewConditionTag);

	//Is adventure component currently loading save file?
	UFUNCTION(BlueprintPure, Category = "Adventure Asset - Save/Loading")
	bool IsLoadingSaveFile() const;
	
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Adventure | Objectives")
	void AddNewObjectiveList(TArray<FAdventureObjectiveEntry> NewObjectives, int64& NewObjectiveUniqueID);

	UFUNCTION(BlueprintCallable, Category = "Adventure | Objectives")
	void AddObjectiveToActiveList(int64 ObjectiveListID, FAdventureObjectiveEntry NewObjective);

	//Just a template for UK2Node_AddObjective
	UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, BlueprintPure, Category = "Adventure | Objectives", meta = (AutoCreateRefTerm="ObjectiveArray"))
	TArray<FAdventureObjectiveEntry> MakeObjectiveArray(TArray<FAdventureObjectiveEntry> ObjectiveArray, FAdventureObjectiveEntry ObjectiveEntry);

	public:

	//The state which the adventure is in
	UPROPERTY(BlueprintReadOnly, Category = "Adventure | State", ReplicatedUsing=OnRep_AdventureState)
	TEnumAsByte<EAdventureState> AdventureState = EAdventureState::Ongoing;

	//The title of this adventure. Mainly for UI.
	//Example: "Orc Slayer"
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Adventure - General Settings", DisplayName = "Title")
	FText AdventureTitle;

	//The description of this adventure, lore of the quest is a good place to put here. Mainly for UI.
	//Example: "Orcs has been planning an attack over at the ravine. We need to kill them before they build a bridge and cross with a whole army."
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Adventure - General Settings", DisplayName = "Description")
	FText AdventureDescription;

	//Optional tags to identify this Adventure
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Adventure - General Settings", DisplayName = "Tags")
	FGameplayTagContainer AdventureTags;

	//The adventures to start once this adventure has been marked as completed.
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Adventure - Quest Chain")
	TArray<TSoftClassPtr<UAdventureAsset>> ChainedAdventures;

	//Is this adventure enabled? If false the adventure will not initialize. Good for temporarily disable adventures.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Adventure - Activation Settings")
	bool bEnabled = true;

	//Has this adventure initialized?
	UPROPERTY(BlueprintReadOnly, Category = "Adventure - Init", DisplayName = "Has Initialized")
	bool bHasInit = false;

	//All the current registered objectives
	UPROPERTY(Replicated, Transient)
	FAdventureObjectiveList ObjectiveList;

	public:

	//Broadcasts on both listen server and client when the Adventure has changed state.
	FAdventureStateModified OnAdventureStateModified;

	public:

	int64 GetNextObjectiveListUniqueID()
	{
		ObjectiveListUniqueID++;
		return ObjectiveListUniqueID;
	};

	private:

	int64 ObjectiveListUniqueID;
	
};

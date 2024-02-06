// Fill out your copyright notice in the Description page of Project Settings.


#include "Nodes/AdventureTask.h"


void UAdventureTask::ActivateTask()
{
	RegisterTaskToGameInstance();
}

void UAdventureTask::DeactivateTask()
{
	UnRegisterTaskFromGameInstance();
}

void UAdventureTask::RegisterTaskToGameInstance()
{
	UWorld* FoundWorld = GEngine->GetWorldFromContextObject(GetOuter(), EGetWorldErrorMode::LogAndReturnNull);
	check(FoundWorld)
	
	if (FoundWorld && FoundWorld->GetGameInstance())
	{
		UGameInstance* GameInstance = FoundWorld->GetGameInstance();
		GameInstance->RegisterReferencedObject(this);
		OwningGameInstance = GameInstance;
	}
}

void UAdventureTask::UnRegisterTaskFromGameInstance()
{
	ensureAlways(OwningGameInstance);
	if (IsValid(OwningGameInstance))
	{
		OwningGameInstance->UnregisterReferencedObject(this);
	}
	#if ENGINE_MAJOR_VERSION == 4
	MarkPendingKill();
	#else
	MarkAsGarbage();
	#endif
}

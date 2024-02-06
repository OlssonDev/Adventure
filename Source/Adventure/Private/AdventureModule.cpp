// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdventureModule.h"
#include "MessageLogModule.h"
#include "Adventure/AdventureGameplayTags.h"
#define LOCTEXT_NAMESPACE "FAdventureModule"

void FAdventureModule::StartupModule()
{
	 FMessageLogModule& MessageLogModule = FModuleManager::LoadModuleChecked<FMessageLogModule>("MessageLog");
	 if (!MessageLogModule.IsRegisteredLogListing("LogAdventure"))
	 {
	 	MessageLogModule.RegisterLogListing("LogAdventure", LOCTEXT("AdventureLabel", "Adventure"));
	 }
}

void FAdventureModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAdventureModule, Adventure)

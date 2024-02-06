// Fill out your copyright notice in the Description page of Project Settings.


#include "FAdventureEditorCommands.h"

#define LOCTEXT_NAMESPACE "AdventureEditorCommands"

void FAdventureEditorCommands::RegisterCommands()
{
	UI_COMMAND(AdventureSettings, "Adventure Settings", "Adventure Settings", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE


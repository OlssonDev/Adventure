// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class FAdventureEditorCommands : public TCommands<FAdventureEditorCommands>
{
	public:
	
	FAdventureEditorCommands() 
		: TCommands<FAdventureEditorCommands>("AdventureEditorCommands", NSLOCTEXT("Contexts", "AdventureEditorCommands", "Adventure Editor Commands"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}
	
	TSharedPtr<FUICommandInfo> AdventureSettings;
	TSharedPtr<FUICommandInfo> ChangeToMultipleObjectives;

	/** Initialize commands */
	virtual void RegisterCommands() override;
};

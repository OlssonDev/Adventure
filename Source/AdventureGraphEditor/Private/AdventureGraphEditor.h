// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <BlueprintEditor.h>

class IAdventureGraphEditor : public FBlueprintEditor
{
	public:

	void InitAdventureEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, class UAdventureAssetBlueprint* InAdventureAsset);
	void InitEventGraph() const;
	
	void CreateAdventureGraph() const;

	// Begin FBlueprintEditor
	virtual void RegisterApplicationModes(const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode, bool bNewlyCreated = false) override;
	virtual void JumpToHyperlink(const UObject* ObjectReference, bool bRequestRename) override;
	virtual bool IsInAScriptingMode() const override;
	virtual TSharedPtr<SGraphEditor> OpenGraphAndBringToFront(UEdGraph* Graph, bool bSetFocus = true) override;
	virtual FGraphAppearanceInfo GetGraphAppearance(class UEdGraph* InGraph) const override;
	virtual TSubclassOf<UEdGraphSchema> GetDefaultSchemaClass() const override;
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>& TabManager) override;
	virtual bool CanAddNewLocalVariable() const override;
	virtual UBlueprint* GetBlueprintObj() const override;
	virtual void SetCurrentMode(FName NewMode) override;
	// End FBlueprintEditor

	void OpenAdventureSettings();
	bool CanModifyAdventureSettings();
	void CreateNewClass(UClass* InClass) const;

	UAdventureAssetBlueprint* GetAdventureBlueprintObj() const;

	static FText GetLocalizedMode(FName InMode);

	public:

	UPROPERTY()
	class UAdventureAssetBlueprint* AdventureBlueprint = nullptr;
	
	TSharedPtr<class FAdventureBlueprintEditorToolbar> AdventureToolbar;

	private:

	/** Palette of Adventure Functions */
	TSharedPtr<class SAdventurePalette> Palette;

	/**	The tab ids for the material editor */
	static const FName PaletteTabId;
};

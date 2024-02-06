#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class UAdventureAssetBlueprint;

class IAdventureGraphEditorModule : public IModuleInterface, public IHasMenuExtensibility, public IHasToolBarExtensibility
{
	public:
	
	virtual void CreateAdventureEditor(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, UAdventureAssetBlueprint* AdventureAsset) {};
	
	virtual TSharedPtr<FExtensibilityManager> GetMenuExtensibilityManager() override { return nullptr; }
	virtual TSharedPtr<FExtensibilityManager> GetToolBarExtensibilityManager() override { return nullptr; }
	
};

#pragma once

#include "CoreMinimal.h"
#include "EdGraphUtilities.h"
#include "Modules/ModuleManager.h"

class FAdventureTasksEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

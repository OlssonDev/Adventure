#pragma once

#include "CoreMinimal.h"
#include "Adventure/AdventureLogChannels.h"

#define ADVENTURE_MESSAGELOG() (FMessageLog("LogAdventure"))

#define ADVENTURE_MESSAGELOG_ADD(x) (FMessageLog("LogAdventure").AddMessage(x));

#define ADVENTURE_ERROR(x) (FAdventureLogMessages::Error(x));

#define ADVENTURE_WARNING(x) (FAdventureLogMessages::Warning(x));

#define ADVENTURE_INFO(x) (FAdventureLogMessages::Info(x));

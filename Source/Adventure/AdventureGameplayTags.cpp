#include "AdventureGameplayTags.h"
#include "GameplayTagsManager.h"

FAdventureGameplayTags FAdventureGameplayTags::AdventureGameplayTags;

FGameplayTag FAdventureGameplayTags::FindTagByString(FString TagString, bool bMatchPartialString)
{
	const UGameplayTagsManager& Manager = UGameplayTagsManager::Get();
	FGameplayTag Tag = Manager.RequestGameplayTag(FName(*TagString), false);

	if (!Tag.IsValid() && bMatchPartialString)
	{
		FGameplayTagContainer AllTags;
		Manager.RequestAllGameplayTags(AllTags, true);

		for (const FGameplayTag TestTag : AllTags)
		{
			if (TestTag.ToString().Contains(TagString))
			{
				UE_LOG(LogTemp, Display, TEXT("Could not find exact match for tag [%s] but found partial match on tag [%s]."), *TagString, *TestTag.ToString());
				Tag = TestTag;
				break;
			}
		}
	}

	return Tag;
}

void FAdventureGameplayTags::AddTags()
{
	AddTag(Adventure_Objective_CanBePreCompleted, "Adventure.Objective.CanBePreCompleted", "If the Adventure Objective can be pre-completed.");
	AddTag(Adventure_Objective_IsOptional, "Adventure.Objective.IsOptional", "If the Adventure Objective is optional.");
	AddTag(Adventure_Objective_HasFailed, "Adventure.Objective.HasFailed", "If the Adventure Objective has failed.");
	AddTag(Adventure_Objective_IsTimed, "Adventure.Objective.IsTimed", "If the adventure objective is timed");
	
	AddTag(Adventure_NoAutoComplete, "Adventure.NoAutoComplete", "If the Adventure should skip the auto complete part when we remove the Adventures and add it to completed adventures list");
}

void FAdventureGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}
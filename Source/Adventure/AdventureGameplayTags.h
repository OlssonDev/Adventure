#pragma once
#include "GameplayTagContainer.h"

struct ADVENTURE_API FAdventureGameplayTags : public FGameplayTagNativeAdder
{
	static FAdventureGameplayTags& Get() { return AdventureGameplayTags; }
	static FGameplayTag FindTagByString(FString TagString, bool bMatchPartialString = false);

	public:

	virtual void AddTags() override;

	protected:
	
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

	public:

	FGameplayTag Adventure_Objective_CanBePreCompleted;
	FGameplayTag Adventure_Objective_IsOptional;
	FGameplayTag Adventure_Objective_HasFailed;
	FGameplayTag Adventure_Objective_IsTimed;
	FGameplayTag Adventure_NoAutoComplete;

	private:

	static FAdventureGameplayTags AdventureGameplayTags;
};

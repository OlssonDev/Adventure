// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveDetailCustomization.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "FAdventureGraphNodeMinimal.h"
#include "Kismet2/BlueprintEditorUtils.h"

#define LOCTEXT_NAMESPACE "FObjectiveDetailCustomization"

FObjectiveDetailCustomization::FObjectiveDetailCustomization()
{
}

void FObjectiveDetailCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	StructPropertyHandle = PropertyHandle;
	HeaderRow
	.NameContent()
	[
		SNew(STextBlock)
		.Font(IDetailLayoutBuilder::GetDetailFont())
		.Text(LOCTEXT("ObjectiveInfo_HeaderName", "Objective Info"))
	];
}

void FObjectiveDetailCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	StructCustomization = &StructCustomizationUtils;
	
	TMap<FName, TSharedPtr<IPropertyHandle>> PropertyHandles;
	uint32 NumChildren;
	StructPropertyHandle->GetNumChildren(NumChildren);

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		TSharedRef<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();
		const FName PropertyName = ChildHandle->GetProperty()->GetFName();
		PropertyHandles.Add(PropertyName, ChildHandle);
	}
	
	ArgumentNameProperty = PropertyHandles.Find(GET_MEMBER_NAME_CHECKED(FAdventureObjectiveEntry, Argument))->ToSharedRef();
	ArgumentTagProperty = PropertyHandles.Find(GET_MEMBER_NAME_CHECKED(FAdventureObjectiveEntry, ArgumentTag))->ToSharedRef();
	
	/*const FText DisplayName = StructPropertyHandle->GetPropertyDisplayName();
	FDetailWidgetRow& ArgumentRow = StructBuilder.AddCustomRow(DisplayName);
	ArgumentRow
	.NameContent()
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Center)
		.AutoWidth()
		[
			SNew(SCheckBox)
			.IsChecked(this, &FObjectiveDetailCustomization::GetCheckBoxState)
			.OnCheckStateChanged(this, &FObjectiveDetailCustomization::OnCheckBoxNewState)
		]
		+ SHorizontalBox::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Center)
		.AutoWidth()
		[
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.ToolTip(IDocumentation::Get()->CreateToolTip(
			LOCTEXT("UseTagArgumentText", "If true, the argument will be a Gameplay Tag instead, otherwise it's Name. Using Gameplay OptionalTags can be enabled by default in the Project Settings under Adventure Settings."),
			NULL,
			TEXT("Shared/Editors/BlueprintEditor"),
			TEXT("AdventureMode")))
			.Text(LOCTEXT("ObjectiveInfo_UseNameAsArgument", "Use Tag Argument"))
		]
	];*/
	
	bool UseGameplayTagAsDefault = UAdventurePluginSettings::IsUsingGameplayTags();
	
	IDetailPropertyRow& ArgumentNameRow = StructBuilder.AddProperty(ArgumentNameProperty.ToSharedRef());
	ArgumentNameRow.Visibility(UseGameplayTagAsDefault ? EVisibility::Collapsed : EVisibility::Visible);
	
	IDetailPropertyRow& ArgumentTagRow = StructBuilder.AddProperty(ArgumentTagProperty.ToSharedRef());
	ArgumentTagRow.Visibility(UseGameplayTagAsDefault ? EVisibility::Visible : EVisibility::Collapsed);
	
	ArgumentTagProperty->SetOnPropertyValueChanged(FSimpleDelegate::CreateLambda([this]
	{
		if (ArgumentTagProperty.IsValid() && ArgumentTagProperty->GetProperty())
		{
			TArray<void*> RawStructData;
			ArgumentTagProperty->AccessRawData(RawStructData);
			
			if (RawStructData.Num() > 0)
			{
				FGameplayTag* Tag = (FGameplayTag*)(RawStructData[0]);
				ArgumentNameProperty->SetValue(Tag->GetTagName());
			}
		}
	}));
	
	for (auto Property : PropertyHandles)
	{
		TSharedRef<IPropertyHandle> RefProperty = Property.Value.ToSharedRef();
		
		if (RefProperty != bUseTagAsArgumentProperty && RefProperty != ArgumentTagProperty && RefProperty != ArgumentNameProperty)
		{
			StructBuilder.AddProperty(RefProperty);
		}
	}
}

void FObjectiveDetailCustomization::OnCheckBoxNewState(ECheckBoxState NewState)
{
	bUseTagAsArgumentProperty->SetValue(NewState == ECheckBoxState::Checked);
	StructCustomization->GetPropertyUtilities()->ForceRefresh();
}

ECheckBoxState FObjectiveDetailCustomization::GetCheckBoxState() const
{
	bool UseGameplayTag;
	bUseTagAsArgumentProperty->GetValue(UseGameplayTag);

	return UseGameplayTag ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

#undef LOCTEXT_NAMESPACE

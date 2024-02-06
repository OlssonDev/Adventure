#include "AdventureClassPicker.h"

#include "ClassViewerModule.h"
#include "EditorStyleSet.h"
#include "IDocumentation.h"
#include "SlateOptMacros.h"
#include "Factories/BlueprintFactory.h"
#include "Widgets/Layout/SUniformGridPanel.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

#define LOCTEXT_NAMESPACE "SAdventureClassPicker"

void SAdventureClassPicker::Construct(const FArguments& InArgs)
{
	
}

bool SAdventureClassPicker::ConfigureProperties(TWeakObjectPtr<UBlueprintFactory> InAdventureObjectiveBlueprintFactory)
{
	bOkClicked = false;
	BlueprintFactory = InAdventureObjectiveBlueprintFactory;
	
	ChildSlot
	[
		SNew(SBorder)
		.Visibility(EVisibility::Visible)
		.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
		[
			SNew(SBox)
			.Visibility(EVisibility::Visible)
			.WidthOverride(1500.0f)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.FillHeight(1)
				[
					SNew(SBorder)
					.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
					.Content()
					[
						SAssignNew(ParentClassContainer, SVerticalBox)
					]
				]
				+ SVerticalBox::Slot()
				.AutoHeight()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Bottom)
				.Padding(8)
				[
					SNew(SButton)
					.HAlign(HAlign_Center)
					.ContentPadding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
					.OnClicked(this, &SAdventureClassPicker::BaseClassedClicked)
					.Text(LOCTEXT("CreateAdventureBlueprintBaseClass", "Create from Base Class"))
					.ToolTip(IDocumentation::Get()->CreateToolTip(
						LOCTEXT("CreateAssetFromBaseClassButton_Tooltip", "Creates an asset based on the base class set in Project Settings -> Adventure Settings."),
						NULL,
						TEXT("Shared/Editors/BlueprintEditor"),
						TEXT("AdventureMode")))
				]
				// Ok/Cancel buttons
				+ SVerticalBox::Slot()
				  .AutoHeight()
				  .HAlign(HAlign_Center)
				  .VAlign(VAlign_Bottom)
				  .Padding(8)
				[
					SNew(SUniformGridPanel)
						.SlotPadding(FEditorStyle::GetMargin("StandardDialog.SlotPadding"))
						.MinDesiredSlotWidth(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotWidth"))
						.MinDesiredSlotHeight(FEditorStyle::GetFloat("StandardDialog.MinDesiredSlotHeight"))
					+ SUniformGridPanel::Slot(0, 0)
					[
						SNew(SButton)
						.HAlign(HAlign_Center)
						.ContentPadding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
						.OnClicked(this, &SAdventureClassPicker::OkClicked)
						.Text(LOCTEXT("AdventureClassPickerOk", "OK"))
					]
					+ SUniformGridPanel::Slot(1, 0)
					[
						SNew(SButton)
						.HAlign(HAlign_Center)
						.ContentPadding(FEditorStyle::GetMargin("StandardDialog.ContentPadding"))
						.OnClicked(this, &SAdventureClassPicker::CancelClicked)
						.Text(LOCTEXT("AdventureClassPickerCancel", "Cancel"))
					]
				]
			]
		]
	];
	
	MakeParentClassPicker();
	
	TSharedRef<SWindow> Window = SNew(SWindow)
			.Title(FText::Format(LOCTEXT("CreateAdventureBlueprintOptions", "Create New {0}"), BlueprintFactory->GetDisplayName()))
			.ClientSize(FVector2D(400, 700))
			.SupportsMinimize(false).SupportsMaximize(false)
	[
		AsShared()
	];

	PickerWindow = Window;

	GEditor->EditorAddModalWindow(Window);
	BlueprintFactory.Reset();
	return bOkClicked;
}

FReply SAdventureClassPicker::BaseClassedClicked()
{
	CloseDialog(true);
	return FReply::Handled();
}

FReply SAdventureClassPicker::OkClicked()
{
	if (BlueprintFactory.IsValid())
	{
		BlueprintFactory->ParentClass = ParentClass.Get();
	}

	CloseDialog(true);

	return FReply::Handled();
}

void SAdventureClassPicker::CloseDialog(bool bWasPicked)
{
	bOkClicked = bWasPicked;
	if (PickerWindow.IsValid())
	{
		PickerWindow.Pin()->RequestDestroyWindow();
	}
}

FReply SAdventureClassPicker::CancelClicked()
{
	CloseDialog();
	return FReply::Handled();
}

FReply SAdventureClassPicker::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::Escape)
	{
		CloseDialog();
		return FReply::Handled();
	}

	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		if (ClassPicked)
		{
			CloseDialog(true);
		}
		else
		{
			BaseClassedClicked();	
		}
	}
	
	return SWidget::OnKeyDown(MyGeometry, InKeyEvent);
}

bool AdventureClassPickerFilter::IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs)
{
	return InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InClass) != EFilterReturn::Failed;
}

bool AdventureClassPickerFilter::IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef<const IUnloadedBlueprintData> InUnloadedClassData, TSharedRef<FClassViewerFilterFuncs> InFilterFuncs)
{
	return InFilterFuncs->IfInChildOfClassesSet(AllowedChildrenOfClasses, InUnloadedClassData) != EFilterReturn::Failed;
}

void SAdventureClassPicker::MakeParentClassPicker()
{
	FClassViewerModule& ClassViewerModule = FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer");
	
	FClassViewerInitializationOptions Options;
	Options.Mode = EClassViewerMode::ClassPicker;
	Options.bIsBlueprintBaseOnly = true;

	TSharedPtr<AdventureClassPickerFilter> Filter = MakeShareable(new AdventureClassPickerFilter);
	
	Filter->AllowedChildrenOfClasses.Add(BlueprintFactory->ParentClass);
	Options.ClassFilter = Filter;
	Options.NameTypeToDisplay = EClassViewerNameTypeToDisplay::DisplayName;

	ParentClassContainer->ClearChildren();
	ParentClassContainer->AddSlot()
	                    .AutoHeight()
	[
		SNew(STextBlock)
				.Text(LOCTEXT("ParentClass", "Search for Class:"))
				.ShadowOffset(FVector2D(1.0f, 1.0f))
	];

	ParentClassContainer->AddSlot()
	[
		ClassViewerModule.CreateClassViewer(Options, FOnClassPicked::CreateSP(this, &SAdventureClassPicker::OnClassPicked))
	];
}

void SAdventureClassPicker::OnClassPicked(UClass* ChosenClass)
{
	ParentClass = ChosenClass;
}

#undef LOCTEXT_NAMESPACE

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

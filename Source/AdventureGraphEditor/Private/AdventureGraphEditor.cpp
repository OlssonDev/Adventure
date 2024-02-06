// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureGraphEditor.h"

#include "AdventureBlueprintEditorToolbar.h"
#include "AdventureEdGraph.h"
#include "AdventureGraphSchema.h"
#include "BlueprintEditorSettings.h"
#include "FAdventureEditorApplicationMode.h"
#include "FAdventureEditorCommands.h"
#include "GraphEditorActions.h"
#include "K2Node_Composite.h"
#include "SBlueprintEditorToolbar.h"
#include "ToolMenus.h"
#include "BlueprintAssets/AdventureAssetBlueprint.h"
#include "AdventureAsset.h"
#include "EditorStyleSet.h"
#include "Engine/TimelineTemplate.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetDebugUtilities.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Toolkits/GlobalEditorCommonCommands.h"
#include "Widgets/Docking/SDockTab.h"
#include "WorkflowOrientedApp/WorkflowUObjectDocuments.h"

#if ENGINE_MAJOR_VERSION == 5
#include "SMyBlueprint.h"
#endif

#define LOCTEXT_NAMESPACE "AdventureEditor"

const FName IAdventureGraphEditor::PaletteTabId(TEXT("AdventureEditor_Palette"));


void IAdventureGraphEditor::InitAdventureEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UAdventureAssetBlueprint* InAdventureAsset)
{
	AdventureBlueprint = InAdventureAsset;
	
	TSharedPtr<IAdventureGraphEditor> ThisPtr(SharedThis(this));
	Toolbar = MakeShareable(new FBlueprintEditorToolbar(ThisPtr));
	AdventureToolbar = MakeShared<FAdventureBlueprintEditorToolbar>(ThisPtr);
	
	InitBlueprintEditor(Mode, InitToolkitHost, {InAdventureAsset}, false);
	RegisterDefaultToolBar();

	ToolkitCommands->MapAction(
	FAdventureEditorCommands::Get().AdventureSettings,
	FExecuteAction::CreateSP(this, &IAdventureGraphEditor::OpenAdventureSettings),
	FCanExecuteAction(),
	FIsActionChecked::CreateSP(this, &IAdventureGraphEditor::CanModifyAdventureSettings));
}

void IAdventureGraphEditor::InitEventGraph() const
{
	const TSharedPtr<SDockTab> DocumentTab = DocumentManager->OpenDocument(FTabPayload_UObject::Make(AdventureBlueprint->UbergraphPages[0]), FDocumentTracker::ForceOpenNewDocument);
	TabManager->SetMainTab(DocumentTab.ToSharedRef());
}

void IAdventureGraphEditor::CreateAdventureGraph() const
{
	if (AdventureBlueprint->AdventureGraph == nullptr)
	{
		AdventureBlueprint->AdventureGraph = FBlueprintEditorUtils::CreateNewGraph(AdventureBlueprint, TEXT("Adventure Graph"), UAdventureEdGraph::StaticClass(), UAdventureGraphSchema::StaticClass());
		
		if (AdventureBlueprint->UbergraphPages.Num())
		{
			FBlueprintEditorUtils::RemoveGraphs(AdventureBlueprint, AdventureBlueprint->UbergraphPages);
		}
		
		FBlueprintEditorUtils::AddUbergraphPage(AdventureBlueprint, AdventureBlueprint->AdventureGraph);
		AdventureBlueprint->AdventureGraph->GetSchema()->CreateDefaultNodesForGraph(*AdventureBlueprint->AdventureGraph);
	}
	
	for (const auto Tab : DocumentManager->GetAllDocumentTabs())
	{
		Tab->RequestCloseTab();
	}
	
	DocumentManager->OpenDocument(FTabPayload_UObject::Make(AdventureBlueprint->AdventureGraph), FDocumentTracker::ForceOpenNewDocument);
}

void IAdventureGraphEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	DocumentManager->SetTabManager(InTabManager);
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);
}

bool IAdventureGraphEditor::CanAddNewLocalVariable() const
{
	return false;
}

UBlueprint* IAdventureGraphEditor::GetBlueprintObj() const
{
	const TArray<UObject*>& EditingObjs = GetEditingObjects();
	for (int32 i = 0; i < EditingObjs.Num(); ++i)
	{
		if (EditingObjs[i]->IsA<UAdventureAssetBlueprint>()) 
		{ 
			return (UBlueprint*)EditingObjs[i]; 
		}
	}
	return nullptr;
}

void IAdventureGraphEditor::SetCurrentMode(FName NewMode)
{
	FBlueprintEditor::SetCurrentMode(NewMode);
}

void IAdventureGraphEditor::OpenAdventureSettings()
{
	StartEditingDefaults( true, true );
}

bool IAdventureGraphEditor::CanModifyAdventureSettings()
{
	return true;
}

void IAdventureGraphEditor::CreateNewClass(UClass* InClass) const
{
	//Currently not in use. Maybe gonna use it in the future for graph asset creation.
	
	/*if(GetAdventureBlueprintObj()->AdventureAsset != nullptr && InClass != nullptr && GetAdventureBlueprintObj()->AdventureAsset->GetOutermost())
	{
		const FString ClassName = FBlueprintEditorUtils::GetClassNameWithoutSuffix(InClass);

		FString PathName = GetAdventureBlueprintObj()->AdventureAsset->GetOutermost()->GetPathName();
		PathName = FPaths::GetPath(PathName);
		
		// Now that we've generated some reasonable default locations/names for the package, allow the user to have the final say
		// before we create the package and initialize the blueprint inside of it.
		FSaveAssetDialogConfig SaveAssetDialogConfig;
		SaveAssetDialogConfig.DialogTitleOverride = LOCTEXT("SaveAssetDialogTitle", "Save Asset As");
		SaveAssetDialogConfig.DefaultPath = PathName;
		SaveAssetDialogConfig.DefaultAssetName = ClassName + TEXT("_New");
		SaveAssetDialogConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::Disallow;

		const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
		const FString SaveObjectPath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveAssetDialogConfig);
		if (!SaveObjectPath.IsEmpty())
		{
			const FString SavePackageName = FPackageName::ObjectPathToPackageName(SaveObjectPath);
			const FString SavePackagePath = FPaths::GetPath(SavePackageName);
			const FString SaveAssetName = FPaths::GetBaseFilename(SavePackageName);

			UPackage* Package = CreatePackage(*SavePackageName);
			if (ensure(Package))
			{
				// Create and init a new Blueprint
				if (UBlueprint* NewBP = FKismetEditorUtilities::CreateBlueprint(InClass, Package, FName(*SaveAssetName), BPTYPE_Normal, UBlueprint::StaticClass(), UBlueprintGeneratedClass::StaticClass()))
				{
					GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(NewBP);

					// Notify the asset registry
					FAssetRegistryModule::AssetCreated(NewBP);

					// Mark the package dirty...
					Package->MarkPackageDirty();
				}
			}
		}
	}

	FSlateApplication::Get().DismissAllMenus();*/
}

void IAdventureGraphEditor::RegisterApplicationModes(const TArray<UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode, bool bNewlyCreated)
{
	const TSharedPtr<IAdventureGraphEditor> ThisPtr(SharedThis(this));
	TArray<TSharedRef<FApplicationMode>> TempModeList {MakeShareable(new FAdventureEditorApplicationMode(ThisPtr)), MakeShareable(new FAdventureBlueprintGraphApplicationMode(ThisPtr)) };
	
	for (const TSharedRef<FApplicationMode>& AppMode : TempModeList)
	{
		AddApplicationMode(AppMode->GetModeName(), AppMode);
	}
	
	SetCurrentMode(FAdventureBlueprintApplicationModes::AdventureMode);
}

void IAdventureGraphEditor::JumpToHyperlink(const UObject* ObjectReference, bool bRequestRename)
{
	if (Cast<const UAdventureEdGraph>(Cast<UEdGraph>(ObjectReference->GetOuter())))
	{
		SetCurrentMode(FAdventureBlueprintApplicationModes::AdventureMode);
	}
	else
	{
		SetCurrentMode(FBlueprintEditorApplicationModes::StandardBlueprintEditorMode);
	}

	if (const UEdGraphNode* Node = Cast<const UEdGraphNode>(ObjectReference))
	{
		if (bRequestRename)
		{
			IsNodeTitleVisible(Node, bRequestRename);
		}
		else
		{
			JumpToNode(Node, false);
		}
	}
	else if (const UEdGraph* Graph = Cast<const UEdGraph>(ObjectReference))
	{
		// Navigating into things should re-use the current tab when it makes sense
		FDocumentTracker::EOpenDocumentCause OpenMode = FDocumentTracker::OpenNewDocument;
		if ((Graph->GetSchema()->GetGraphType(Graph) == GT_Ubergraph) || Cast<UK2Node_Composite>(Graph->GetOuter()))
		{
			// Ubergraphs directly reuse the current graph
			OpenMode = FDocumentTracker::NavigatingCurrentDocument;
		}
		else
		{
			// Walk up the outer chain to see if any tabs have a parent of this document open for edit, and if so
			// we should reuse that one and drill in deeper instead
			for (UObject* WalkPtr = const_cast<UEdGraph*>(Graph); WalkPtr != nullptr; WalkPtr = WalkPtr->GetOuter())
			{
				TArray< TSharedPtr<SDockTab> > TabResults;
				if (FindOpenTabsContainingDocument(WalkPtr, /*out*/ TabResults))
				{
					// See if the parent was active
					bool bIsActive = false;
					for (TSharedPtr<SDockTab> Tab : TabResults)
					{
						if (Tab->IsActive())
						{
							bIsActive = true;
							break;
						}
					}

					if (bIsActive)
					{
						OpenMode = FDocumentTracker::NavigatingCurrentDocument;
						break;
					}
				}
			}
		}

		// Force it to open in a new document if shift is pressed
		const bool bIsShiftPressed = FSlateApplication::Get().GetModifierKeys().IsShiftDown();
		if (bIsShiftPressed)
		{
			OpenMode = FDocumentTracker::ForceOpenNewDocument;
		}

		// Open the document
		OpenDocument(Graph, OpenMode);
	}
	else if(const UFunction* Function = Cast<const UFunction>(ObjectReference))
	{
		UBlueprint* BP = GetBlueprintObj();
		if(BP)
		{
			if (UEdGraph* FunctionGraph = FBlueprintEditorUtils::FindScopeGraph(BP, Function))
			{
				OpenDocument(FunctionGraph, FDocumentTracker::OpenNewDocument);
			}
		}
	}
	else if(const UBlueprintGeneratedClass* Class = Cast<const UBlueprintGeneratedClass>(ObjectReference))
	{
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(Class->ClassGeneratedBy);
	}
	else if (const UTimelineTemplate* Timeline = Cast<const UTimelineTemplate>(ObjectReference))
	{
		OpenDocument(Timeline, FDocumentTracker::OpenNewDocument);
	}
	else if ((ObjectReference != nullptr) && ObjectReference->IsAsset())
	{
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(const_cast<UObject*>(ObjectReference));
	}
	else
	{
		UE_LOG(LogBlueprint, Warning, TEXT("Unknown type of hyperlinked object (%s), cannot focus it"), *GetNameSafe(ObjectReference));
	}

	//@TODO: Hacky way to ensure a message is seen when hitting an exception and doing intraframe debugging
	const FText ExceptionMessage = FKismetDebugUtilities::GetAndClearLastExceptionMessage();
	if (!ExceptionMessage.IsEmpty())
	{
		LogSimpleMessage( ExceptionMessage );
	}
}

bool IAdventureGraphEditor::IsInAScriptingMode() const
{
	return FBlueprintEditor::IsInAScriptingMode() || GetCurrentMode() == FAdventureBlueprintApplicationModes::AdventureMode;
}

TSharedPtr<SGraphEditor> IAdventureGraphEditor::OpenGraphAndBringToFront(UEdGraph* Graph, bool bSetFocus)
{
	if (!Graph || Graph->IsPendingKill())
	{
		return TSharedPtr<SGraphEditor>();
	}
	
	if (Cast<UAdventureEdGraph>(Cast<UEdGraph>(Graph)))
	{
		SetCurrentMode(FAdventureBlueprintApplicationModes::AdventureMode);
	}
	else
	{
		SetCurrentMode(FBlueprintEditorApplicationModes::StandardBlueprintEditorMode);
	}
	
	TSharedPtr<SDockTab> TabWithGraph = OpenDocument(Graph, FDocumentTracker::OpenNewDocument);
	TSharedRef<SGraphEditor> NewGraphEditor = StaticCastSharedRef<SGraphEditor>(TabWithGraph->GetContent());
	
	if (bSetFocus)
	{
		NewGraphEditor->CaptureKeyboard();
	}

	return NewGraphEditor;
}

FGraphAppearanceInfo IAdventureGraphEditor::GetGraphAppearance(UEdGraph* InGraph) const
{
	FGraphAppearanceInfo AppearanceInfo = FBlueprintEditor::GetGraphAppearance(InGraph);
	AppearanceInfo.CornerText = LOCTEXT("AppearanceCornerText", "ADVENTURE");
	AppearanceInfo.ReadOnlyText = LOCTEXT("AppearanceCornerText", "ADVENTURE");
	AppearanceInfo.CornerImage = FEditorStyle::GetBrush("UMGEditor.SwitchToDesigner");
	AppearanceInfo.PIENotifyText = AppearanceInfo.ReadOnlyText;

	return AppearanceInfo;
}

TSubclassOf<UEdGraphSchema> IAdventureGraphEditor::GetDefaultSchemaClass() const
{
	return UAdventureGraphSchema::StaticClass();
}

UAdventureAssetBlueprint* IAdventureGraphEditor::GetAdventureBlueprintObj() const
{
	return CastChecked<UAdventureAssetBlueprint>(GetBlueprintObj());
}

FText IAdventureGraphEditor::GetLocalizedMode(FName InMode)
{
	static TMap< FName, FText > LocModes;

	if (LocModes.Num() == 0)
	{
		LocModes.Add("AdventureEditorMode", LOCTEXT("AdventureEditorMode", "Adventure Graph"));
	}

	check(InMode != NAME_None);
	const FText* OutDesc = LocModes.Find(InMode);
	check(OutDesc);
	return *OutDesc;
}

#undef LOCTEXT_NAMESPACE

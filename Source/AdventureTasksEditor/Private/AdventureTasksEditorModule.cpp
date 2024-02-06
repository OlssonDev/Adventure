#include "AdventureTasksEditorModule.h"

#include "AdventureTasksNodes/K2Node_AddObjective.h"
#include "AdventureTasksNodes/Slate/SGraphNode_AddObjective.h"

#define LOCTEXT_NAMESPACE "FAdventureTasksEditorModule"

class FGraphPanelNodeFactory_AdventureTasks final : public FGraphPanelNodeFactory
{
	virtual TSharedPtr<class SGraphNode> CreateNode(UEdGraphNode* Node) const override
	{
		if (UK2Node_AddObjective* BTNode = Cast<UK2Node_AddObjective>(Node))
		{
			return SNew(SGraphNode_AddObjective, BTNode);
		}

		return nullptr;
	}
};

void FAdventureTasksEditorModule::StartupModule()
{
	FEdGraphUtilities::RegisterVisualNodeFactory(MakeShareable(new FGraphPanelNodeFactory_AdventureTasks()));
}

void FAdventureTasksEditorModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FAdventureTasksEditorModule, AdventureTasksEditor)
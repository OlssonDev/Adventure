#include "AdventureMessageLogEditor.h"

#include "AdventureMacros.h"
#include "Kismet2/KismetDebugUtilities.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Misc/UObjectToken.h"

void AdventureMessageLogEditor::LogMessage(const TSharedRef<FTokenizedMessage>& Message)
{
	struct Local
	{
		static void OnMessageLogLinkActivated(const class TSharedRef<IMessageToken>& Token)
		{
			if (Token->GetType() == EMessageToken::Object)
			{
				const TSharedRef<FUObjectToken> UObjectToken = StaticCastSharedRef<FUObjectToken>(Token);
				if (UObjectToken->GetObject().IsValid())
				{
					FKismetEditorUtilities::BringKismetToFocusAttentionOnObject(UObjectToken->GetObject().Get());
				}
			}
		}
	};

	const TArray<const FFrame*>& ScriptStack = FBlueprintContextTracker::Get().GetScriptStack();
	
	TArray<TSharedPtr<IMessageToken>> ReversedTokens;
	if (ScriptStack.Num() > 0)
	{
		const FFrame& StackFrame = *ScriptStack.Last();
		UClass* ClassContainingCode = FKismetDebugUtilities::FindClassForNode(nullptr, StackFrame.Node);
		UBlueprint* BlueprintObj = (ClassContainingCode ? Cast<UBlueprint>(ClassContainingCode->ClassGeneratedBy) : NULL);
		if (BlueprintObj)
		{
			const int32 BreakpointOffset = StackFrame.Code - StackFrame.Node->Script.GetData() - 1;

			ReversedTokens.Add(FUObjectToken::Create(BlueprintObj, FText::FromString(BlueprintObj->GetName()))
				->OnMessageTokenActivated(FOnMessageTokenActivated::CreateStatic(&Local::OnMessageLogLinkActivated))
			);
			ReversedTokens.Add(FTextToken::Create(FText::FromString("Blueprint: ")));

			// NOTE: StackFrame.Node is not a blueprint node like you may think ("Node" has some legacy meaning)
			ReversedTokens.Add(FUObjectToken::Create(StackFrame.Node, StackFrame.Node->GetDisplayNameText())
				->OnMessageTokenActivated(FOnMessageTokenActivated::CreateStatic(&Local::OnMessageLogLinkActivated))
			);
			ReversedTokens.Add(FTextToken::Create(FText::FromString("Function: ")));

			#if WITH_EDITORONLY_DATA // to protect access to GeneratedClass->DebugData
			UBlueprintGeneratedClass* GeneratedClass = Cast<UBlueprintGeneratedClass>(ClassContainingCode);
			if ((GeneratedClass != NULL) && GeneratedClass->DebugData.IsValid())
			{
				UEdGraphNode* BlueprintNode = GeneratedClass->DebugData.FindSourceNodeFromCodeLocation(StackFrame.Node, BreakpointOffset, true);
				// if instead, there is a node we can point to...
				if (BlueprintNode != NULL)
				{
					ReversedTokens.Add(FUObjectToken::Create(BlueprintNode->GetGraph(), FText::FromString(GetNameSafe(BlueprintNode->GetGraph())))
						->OnMessageTokenActivated(FOnMessageTokenActivated::CreateStatic(&Local::OnMessageLogLinkActivated))
					);
					ReversedTokens.Add(FTextToken::Create(FText::FromString("Graph: ")));

					ReversedTokens.Add(FUObjectToken::Create(BlueprintNode, BlueprintNode->GetNodeTitle(ENodeTitleType::ListView))
						->OnMessageTokenActivated(FOnMessageTokenActivated::CreateStatic(&Local::OnMessageLogLinkActivated))
					);
					ReversedTokens.Add(FTextToken::Create(FText::FromString("Node: ")));
				}
			}
			#endif // WITH_EDITORONLY_DATA
		}
	}
	for (int32 Index = ReversedTokens.Num() - 1; Index >= 0; --Index)
	{
		Message->AddToken(ReversedTokens[Index].ToSharedRef());
	}

	ADVENTURE_MESSAGELOG_ADD(Message)
}

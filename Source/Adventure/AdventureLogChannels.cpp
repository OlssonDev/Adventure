#include "AdventureLogChannels.h"
#include "AdventureMacros.h"
#include "Misc/UObjectToken.h"

DEFINE_LOG_CATEGORY(LogAdventure);

FString GetClientServerContextString(UObject* ContextObject)
{
	ENetRole Role = ROLE_None;

	if (AActor* Actor = Cast<AActor>(ContextObject))
	{
		Role = Actor->GetLocalRole();
	}
	else if (UActorComponent* Component = Cast<UActorComponent>(ContextObject))
	{
		Role = Component->GetOwnerRole();
	}

	if (Role != ROLE_None)
	{
		return (Role == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
	}
	else
	{
#if WITH_EDITOR
		if (GIsEditor)
		{
			extern ENGINE_API FString GPlayInEditorContextString;
			return GPlayInEditorContextString;
		}
#endif
	}

	return TEXT("[]");
}

FAdventureLogMessages::FLogMessageDelegate FAdventureLogMessages::LogMessageDelegate;

void FAdventureLogMessages::LogMessage(const TSharedRef<FTokenizedMessage>& Message)
{
	if (LogMessageDelegate.IsBound())
	{
		LogMessageDelegate.Broadcast(Message);
	}
	else
	{
		ADVENTURE_MESSAGELOG_ADD(Message);
	}
}

void FAdventureLogMessages::LogMessage(const FText& Message, EMessageSeverity::Type Severity, const UObject* Object)
{
	TSharedRef<FTokenizedMessage> NewMessage = FTokenizedMessage::Create(Severity);
	if (IsValid(Object))
	{
		NewMessage->AddToken(FUObjectToken::Create(Object));
		NewMessage->AddToken(FTextToken::Create(FText::FromString(": ")));
	}
	NewMessage->AddToken(FTextToken::Create(Message));
	LogMessage(NewMessage);
}

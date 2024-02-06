#pragma once

#include "CoreMinimal.h"
#include "Logging/TokenizedMessage.h"

ADVENTURE_API DECLARE_LOG_CATEGORY_EXTERN(LogAdventure, Log, All);

#define LogAdventure(Verbosity, Format, ...) UE_LOG(LogAdventure, Verbosity, Format, ##__VA_ARGS__)

ADVENTURE_API FString GetClientServerContextString(UObject* ContextObject = nullptr);

struct ADVENTURE_API FAdventureLogMessages
{
	DECLARE_MULTICAST_DELEGATE_OneParam(FLogMessageDelegate, const TSharedRef<FTokenizedMessage>&);
	static FLogMessageDelegate LogMessageDelegate;
	
	static void LogMessage(const TSharedRef<FTokenizedMessage>& Message);
	static void LogMessage(const FText& Message, EMessageSeverity::Type Severity, const UObject* Object = nullptr);
	
public:
	
	static void Error(const FString& Message, const UObject* Object = nullptr)
	{
		Error(FText::FromString(Message), Object);
	}
	static void Error(const FText& Message, const UObject* Object = nullptr)
	{
		LogMessage(Message, EMessageSeverity::Error, Object);
	}
	
	static void Warning(const FString& Message, const UObject* Object = nullptr)
	{
		Warning(FText::FromString(Message), Object);
	}
	static void Warning(const FText& Message, const UObject* Object = nullptr)
	{
		LogMessage(Message, EMessageSeverity::Warning, Object);
	}
	
	static void Info(const FString& Message, const UObject* Object = nullptr)
	{
		Info(FText::FromString(Message), Object);
	}
	static void Info(const FText& Message, const UObject* Object = nullptr)
	{
		LogMessage(Message, EMessageSeverity::Info, Object);
	}
};

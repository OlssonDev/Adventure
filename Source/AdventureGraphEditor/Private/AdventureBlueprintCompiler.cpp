// Fill out your copyright notice in the Description page of Project Settings.


#include "AdventureBlueprintCompiler.h"
#include "BlueprintAssets/AdventureAssetBlueprint.h"
#include "AdventureAsset.h"
#include "AdventureBlueprintGeneratedClass.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetReinstanceUtilities.h"

#define LOCTEXT_NAMESPACE "AdventureCompilerContext"

bool FAdventureBlueprintCompiler::CanCompile(const UBlueprint* Blueprint)
{
	return Cast<UAdventureAssetBlueprint>(Blueprint) != nullptr;
}

void FAdventureBlueprintCompiler::Compile(UBlueprint* Blueprint, const FKismetCompilerOptions& CompileOptions, FCompilerResultsLog& Results)
{
	if (UAdventureAssetBlueprint* QuestBlueprint = CastChecked<UAdventureAssetBlueprint>(Blueprint))
	{
		FAdventureCompilerContext Compiler(QuestBlueprint, Results, CompileOptions);
		Compiler.Compile();
		check(Compiler.NewClass);
	}
}

bool FAdventureBlueprintCompiler::GetBlueprintTypesForClass(UClass* ParentClass, UClass*& OutBlueprintClass, UClass*& OutBlueprintGeneratedClass) const
{
	if (ParentClass == UAdventureAsset::StaticClass() || ParentClass->IsChildOf(UAdventureAsset::StaticClass()))
	{
		OutBlueprintClass = UAdventureAssetBlueprint::StaticClass();
		OutBlueprintGeneratedClass = UAdventureBlueprintGeneratedClass::StaticClass();
		return true;
	}

	return false;
}

FAdventureBlueprintCompiler::FAdventureBlueprintCompiler()
{
}

FAdventureCompilerContext::FAdventureCompilerContext(UAdventureAssetBlueprint* SourceAdventureBP, FCompilerResultsLog& InMessageLog, const FKismetCompilerOptions& InCompilerOptions) : Super(SourceAdventureBP, InMessageLog, InCompilerOptions)
{
}

void FAdventureCompilerContext::FinishCompilingClass(UClass* Class)
{
	FKismetCompilerContext::FinishCompilingClass(Class);
}

void FAdventureCompilerContext::EnsureProperGeneratedClass(UClass*& TargetUClass)
{
	if (TargetUClass && !((UObject*)TargetUClass)->IsA(UAdventureBlueprintGeneratedClass::StaticClass()))
	{
		FKismetCompilerUtilities::ConsignToOblivion(TargetUClass, Blueprint->bIsRegeneratingOnLoad);
		TargetUClass = nullptr;
	}
}

void FAdventureCompilerContext::SpawnNewClass(const FString& NewClassName)
{
	NewAdventureBlueprintClass = FindObject<UAdventureBlueprintGeneratedClass>(Blueprint->GetOutermost(), *NewClassName);

	if ( NewAdventureBlueprintClass == nullptr )
	{
		NewAdventureBlueprintClass = NewObject<UAdventureBlueprintGeneratedClass>(Blueprint->GetOutermost(), FName(*NewClassName), RF_Public | RF_Transactional);
	}
	else
	{
		// Already existed, but wasn't linked in the Blueprint yet due to load ordering issues
		FBlueprintCompileReinstancer::Create(NewAdventureBlueprintClass);
	}
	
	NewClass = NewAdventureBlueprintClass;
}

void FAdventureCompilerContext::CompileFunction(FKismetFunctionContext& Context)
{
	FKismetCompilerContext::CompileFunction(Context);
}

#undef LOCTEXT_NAMESPACE




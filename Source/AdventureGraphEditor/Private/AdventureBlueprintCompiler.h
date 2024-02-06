// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KismetCompiler.h"
#include "KismetCompilerModule.h"

class UAdventureAssetBlueprint;

//Currently empty. Maybe need it in the future. Mostly for future proofing.
class FAdventureBlueprintCompiler : public IBlueprintCompiler
{
	public:

	virtual bool CanCompile(const UBlueprint* Blueprint) override;
	virtual void Compile(UBlueprint* Blueprint, const FKismetCompilerOptions& CompileOptions, FCompilerResultsLog& Results) override;
	virtual bool GetBlueprintTypesForClass(UClass* ParentClass, UClass*& OutBlueprintClass, UClass*& OutBlueprintGeneratedClass) const override;

	FAdventureBlueprintCompiler();
};

class FAdventureCompilerContext : public FKismetCompilerContext
{
	public:

	FAdventureCompilerContext(UAdventureAssetBlueprint* SourceAdventureBP, FCompilerResultsLog& InMessageLog, const FKismetCompilerOptions& InCompilerOptions);
	
	virtual void FinishCompilingClass(UClass* Class) override;
	virtual void EnsureProperGeneratedClass(UClass*& TargetClass) override;
	virtual void SpawnNewClass(const FString& NewClassName) override;
	virtual void CompileFunction(FKismetFunctionContext& Context) override;

	protected:

	typedef FKismetCompilerContext Super;

	class UAdventureBlueprintGeneratedClass* NewAdventureBlueprintClass;
};

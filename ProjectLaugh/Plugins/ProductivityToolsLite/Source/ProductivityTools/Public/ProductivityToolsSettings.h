// Copyright (c) 2019 Isara Technologies. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ProductivityToolsSettings.generated.h"

UENUM()
namespace EExistingPrefixMode
{
	enum Type {
		REPLACE					UMETA(DisplayName = "Replace"),
		ADD_BEFORE				UMETA(DisplayName = "Add before"),
		LEAVE_UNCHANGED			UMETA(DisplayName = "Leave unchanged"),
	};
}

USTRUCT()
struct FClassNameToPrefix 
{
	GENERATED_BODY()

public:
	/** The name of the class to add a prefix */
	UPROPERTY(config, EditAnywhere, Category = FixNaming)
	FString ClassName;

	/** The prefix to use */
	UPROPERTY(config, EditAnywhere, Category = FixNaming)
	FString Prefix;
};

USTRUCT()
struct FClassGroupToPrefix
{
	GENERATED_BODY()

public:
	/** The name the classes must contain to add a prefix */
	UPROPERTY(config, EditAnywhere, Category = FixNaming)
	FString ClassGroupName;

	/** The prefix to use */
	UPROPERTY(config, EditAnywhere, Category = FixNaming)
	FString Prefix;
};

#define SETTINGS UProductivityToolsSettings::Settings

UCLASS(config = Engine, defaultconfig)
class UProductivityToolsSettings  : public UObject
{
	GENERATED_BODY()

public:
	// CREATE PACKAGE

	/** The name of the package to create */
	UPROPERTY(config, EditAnywhere, Category = CreatePackage)
	FString PackageName;

	/** The sub folders to create */
	UPROPERTY(config, EditAnywhere, Category = CreatePackage)
	TArray<FString> SubFoldersToCreate;

	// FIX NAMING

	/** The behavior of the fix naming when an already existing prefix is found */
	UPROPERTY(config, EditAnywhere, Category = FixNaming)
	TEnumAsByte<EExistingPrefixMode::Type> ExistingPrefixMode;

	/** The prefixes to add for each class when fix naming */
	UPROPERTY(config, EditAnywhere, Category = FixNaming)
	TArray<FClassNameToPrefix> Prefixes;

	/** The prefixes to add to class groups when fix naming */
	UPROPERTY(config, EditAnywhere, Category = FixNaming)
	TArray<FClassGroupToPrefix> PrefixesGroup;

	/** Enable the addition of suffixes when fix naming (Experimental) */
	UPROPERTY(config, EditAnywhere, Category = FixNaming)
	bool bEnableSuffixes = false;

	// ORGANIZE PACKAGE

	/** Mapping an asset type to its corresponding folder (the key is the asset type) */
	UPROPERTY(config, EditAnywhere, Category = Organize)
	TMap<FString, FString> OrganizeGroups;

	void Reset();


	/** The plug-in Settings retrieved from the project settings */
	static UProductivityToolsSettings const* Settings;

	UProductivityToolsSettings(const FObjectInitializer& ObjectInitializer);
};
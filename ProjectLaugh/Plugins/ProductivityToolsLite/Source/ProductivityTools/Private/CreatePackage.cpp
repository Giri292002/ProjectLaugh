// Copyright (c) 2019 Isara Technologies. All Rights Reserved.

#include "CreatePackage.h"
#include "ProductivityToolsSettings.h"
#include "Tools.h"

#include "HAL/PlatformFileManager.h"
#include "HAL/FileManager.h"
#include "Modules/ModuleManager.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/Paths.h"
#include "ContentBrowserModule.h"


#define LOCTEXT_NAMESPACE "FProductivityToolsModule"


void FCreatePackage::CreatePackage(TArray<FString> SelectedPaths)
{
	// For each selected path
	for (FString SelectedPath : SelectedPaths)
	{
		FString PackageName = SETTINGS->PackageName;

		// Construct the full path to the package that will be created
		FString PackageFullPath = FTools::GetFullPathFromContentBrowserPath(SelectedPath) / PackageName;

		int32 count = 0;
		// if a folder named "NewPackage" already exists, choose "NewPackage1" as folder name ...
		while (FPaths::DirectoryExists(PackageFullPath))
		{
			count++;
			PackageName = SETTINGS->PackageName + FString::FromInt(count);
			PackageFullPath = FTools::GetFullPathFromContentBrowserPath(SelectedPath) / PackageName;
		}

		// for each sub folder to create in the project settings
		for (FString SubFolderName : SETTINGS->SubFoldersToCreate)
		{
			// Construct the path to the sub folder to create
			const FString NewPath = SelectedPath / PackageName / SubFolderName;

			// Make sure the folder exists on disk
			FString NewPathOnDisk;
			bool bSuccess = FPackageName::TryConvertLongPackageNameToFilename(NewPath, NewPathOnDisk) &&
				IFileManager::Get().MakeDirectory(*NewPathOnDisk, true);
			if (bSuccess)
			{
				// Create the specified path
				FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
				bSuccess = AssetRegistryModule.Get().AddPath(NewPath);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
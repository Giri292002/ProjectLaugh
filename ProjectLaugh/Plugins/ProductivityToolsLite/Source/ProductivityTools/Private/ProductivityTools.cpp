// Copyright (c) 2019 Isara Technologies. All Rights Reserved.

#include "ProductivityTools.h"
#include "ProductivityToolsStyle.h"
#include "ProductivityToolsCommands.h"
#include "ProductivityToolsSettings.h"

#include "CreatePackage.h"
#include "OrganizePackage.h"
#include "FixNaming.h"
#include "Tools.h"

#include "ContentBrowserModule.h"

#include "ISettingsModule.h"
#include "ISettingsSection.h"

#include "ConsolidateWindow.h"
#include "UnrealEd.h"

static const FName ProductivityToolsTabName("ProductivityTools");

#define LOCTEXT_NAMESPACE "FProductivityToolsModule"


void FProductivityToolsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FProductivityToolsStyle::Initialize();
	FProductivityToolsStyle::ReloadTextures();

	FProductivityToolsCommands::Register();

	SelectedAssets = TArray<FAssetData>();
	
	ISettingsModule* const SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	if (SettingsModule != nullptr)
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project", "Plugins", "ProductivityTools",
			LOCTEXT("ProductivityToolsSettingsName", "Productivity Tools"),
			LOCTEXT("ProductivityToolsSettingsDescription", "Configure the Productivity Tools plug-in."),
			GetMutableDefault<UProductivityToolsSettings>()
		);

		auto ResetLambda = []() { GetMutableDefault<UProductivityToolsSettings>()->Reset(); return true; };
		SettingsSection->OnResetDefaults().BindLambda(ResetLambda);
	}

	// Retrieve the specified settings in the project settings
	UProductivityToolsSettings::Settings = GetDefault<UProductivityToolsSettings>();

	PluginCommands = MakeShareable(new FUICommandList);

	// Retrieve the ContentBrowserModule
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	
	// Extend the Asset Selection Context Menu
	TArray<FContentBrowserMenuExtender_SelectedAssets>& CBAssetMenuExtenderDelegates = ContentBrowserModule.GetAllAssetViewContextMenuExtenders();
	CBAssetMenuExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedAssets::CreateRaw(this, &FProductivityToolsModule::OnExtendContentBrowserAssetSelectionMenu));
	ContentBrowserAssetExtenderDelegateHandle = CBAssetMenuExtenderDelegates.Last().GetHandle();

	// Extend the Path Selection Context Menu
	TArray<FContentBrowserMenuExtender_SelectedPaths>& CBPathExtenderDelegates = ContentBrowserModule.GetAllPathViewContextMenuExtenders();
	CBPathExtenderDelegates.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FProductivityToolsModule::OnExtendContentBrowserPathSelectionMenu));
	ContentBrowserPathExtenderDelegateHandle = CBPathExtenderDelegates.Last().GetHandle();

	// Extend the Content Browser Commands
	TArray<FContentBrowserCommandExtender>& CBCommandExtenderDelegates = ContentBrowserModule.GetAllContentBrowserCommandExtenders();
	CBCommandExtenderDelegates.Add(FContentBrowserCommandExtender::CreateRaw(this, &FProductivityToolsModule::OnExtendContentBrowserCommands));
	ContentBrowserCommandExtenderDelegateHandle = CBCommandExtenderDelegates.Last().GetHandle();
}

TSharedRef<FExtender> FProductivityToolsModule::OnExtendContentBrowserAssetSelectionMenu(const TArray<FAssetData>& selectedAssets)
{
	// Retrieve the selected assets in the content browser
	this->SelectedAssets = selectedAssets;
	this->SelectedPaths.Empty();

	// Create an extension in the Asset Selection Context Menu after the AssetContextReferences section
	TSharedRef<FExtender> Extender(new FExtender());
	Extender->AddMenuExtension(
		"AssetContextReferences",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateRaw(this, &FProductivityToolsModule::CreateAssetSelectionContextMenu));

	return Extender;
}

TSharedRef<FExtender> FProductivityToolsModule::OnExtendContentBrowserPathSelectionMenu(const TArray<FString>& selectedPaths)
{
	// Retrieve the selected assets in the content browser
	this->SelectedPaths = selectedPaths;
	this->SelectedAssets.Empty();

	// Create an extension in the Path Selection Context Menu after the PathContextBulkOperations section
	TSharedRef<FExtender> Extender(new FExtender());
	Extender->AddMenuExtension(
		"PathContextBulkOperations",
		EExtensionHook::After,
		nullptr,
		FMenuExtensionDelegate::CreateRaw(this, &FProductivityToolsModule::CreatePathSelectionContextMenu));

	return Extender;
}

void FProductivityToolsModule::OnExtendContentBrowserCommands(TSharedRef<FUICommandList> CommandList, FOnContentBrowserGetSelection GetSelectionDelegate)
{
	// Map a function to the CreatePackage button
	CommandList->MapAction(FProductivityToolsCommands::Get().CreatePackage,
		FExecuteAction::CreateLambda([this]
		{
			FCreatePackage::CreatePackage(SelectedPaths);
		})
	);

	// Map a function to the OrganizePackage button
	CommandList->MapAction(FProductivityToolsCommands::Get().OrganizePackage,
		FExecuteAction::CreateLambda([this]
		{
			FOrganizePackage::OrganizePackage(SelectedAssets, SelectedPaths);
		})
	);

	// Map a function to the FixNaming button
	CommandList->MapAction(FProductivityToolsCommands::Get().FixNaming,
		FExecuteAction::CreateLambda([this]
		{
			FFixNaming::FixNaming(SelectedAssets, SelectedPaths);
		})
	);
}

void FProductivityToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FProductivityToolsStyle::Shutdown();

	FProductivityToolsCommands::Unregister();
}

void FProductivityToolsModule::CreateAssetSelectionContextMenu(FMenuBuilder& Builder)
{
	// Add a section for Productivity Tools in Asset Selection Context Menu
	Builder.BeginSection("AssetContextProductivityTools", LOCTEXT("ProductivityToolsMenuHeading", "ProductivityTools"));
	{
		// Add the OrganizePackage button
		Builder.AddMenuEntry(FProductivityToolsCommands::Get().OrganizePackage);

		// Add the Fix Naming button
		Builder.AddMenuEntry(FProductivityToolsCommands::Get().FixNaming);	
	}
	Builder.EndSection();
}

void FProductivityToolsModule::CreatePathSelectionContextMenu(FMenuBuilder& Builder)
{
	// Add a section for Productivity Tools in Path Selection Context Menu
	Builder.BeginSection("PathContextProductivityTools", LOCTEXT("ProductivityToolsMenuHeading", "ProductivityTools"));
	{
		// Add the CreatePackage button
		Builder.AddMenuEntry(FProductivityToolsCommands::Get().CreatePackage);	

		// Add the OrganizePackage button
		Builder.AddMenuEntry(FProductivityToolsCommands::Get().OrganizePackage);

		// Add the Fix Naming button
		Builder.AddMenuEntry(FProductivityToolsCommands::Get().FixNaming);
	}
	Builder.EndSection();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FProductivityToolsModule, ProductivityTools)
// Copyright (c) 2019 Isara Technologies. All Rights Reserved.

#include "ProductivityToolsCommands.h"

#define LOCTEXT_NAMESPACE "FProductivityToolsModule"

void FProductivityToolsCommands::RegisterCommands()
{
	UI_COMMAND(CreatePackage, "Create Package",
		"Create a folder with a configured preset of sub-folders",
		EUserInterfaceActionType::Button,
		FInputChord());

	UI_COMMAND(OrganizePackage, "Organize Package",
		"Split selected assets into folders according to their types",
		EUserInterfaceActionType::Button,
		FInputChord());

	UI_COMMAND(FixNaming, "Fix Naming",
		"Apply Assets naming convention to selected Assets",
		EUserInterfaceActionType::Button,
		FInputChord());
}

#undef LOCTEXT_NAMESPACE

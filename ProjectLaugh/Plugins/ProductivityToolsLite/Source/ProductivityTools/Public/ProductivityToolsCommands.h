// Copyright (c) 2019 Isara Technologies. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ProductivityToolsStyle.h"

class FProductivityToolsCommands : public TCommands<FProductivityToolsCommands>
{
public:

	FProductivityToolsCommands()
		: TCommands<FProductivityToolsCommands>(TEXT("ProductivityTools"), NSLOCTEXT("Contexts", "ProductivityTools", "ProductivityTools Plugin"), NAME_None, FProductivityToolsStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > CreatePackage;

	TSharedPtr< FUICommandInfo > OrganizePackage;

	TSharedPtr< FUICommandInfo > FixNaming;
};

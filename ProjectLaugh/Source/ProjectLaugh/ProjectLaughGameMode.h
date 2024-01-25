// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectLaughGameMode.generated.h"

UCLASS(minimalapi)
class AProjectLaughGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectLaughGameMode();

	virtual void BeginPlay() override;
};




// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectLaughGameMode.h"
#include "ProjectLaughCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectLaughGameMode::AProjectLaughGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AProjectLaughGameMode::BeginPlay()
{
	Super::BeginPlay();
}

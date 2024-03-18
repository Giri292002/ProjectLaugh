// Fill out your copyright notice in the Description page of Project Settings.


#include "PLMainMenuGameMode.h"
#include "ProjectLaugh/Core/MainMenu/PLMainMenuPawn.h"
#include "ProjectLaugh/Core/PLEOSGameInstance.h"

APLMainMenuGameMode::APLMainMenuGameMode()
{
	DefaultPawnClass = APLMainMenuPawn::StaticClass();
}

void APLMainMenuGameMode::BeginPlay()
{
	UPLEOSGameInstance* PLEOSGameInstance = GetGameInstance<UPLEOSGameInstance>();
	check(PLEOSGameInstance);
	PLEOSGameInstance->DestroySession();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PLMainMenuGameMode.h"
#include "ProjectLaugh/Core/MainMenu/PLMainMenuPawn.h"

APLMainMenuGameMode::APLMainMenuGameMode()
{
	DefaultPawnClass = APLMainMenuPawn::StaticClass();
}

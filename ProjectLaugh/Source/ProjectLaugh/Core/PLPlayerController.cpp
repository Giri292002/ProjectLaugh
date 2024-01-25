// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerController.h"

void APLPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

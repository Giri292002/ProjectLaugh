// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerState.h"

#include "PLPlayerController.h"
#include "ProjectLaugh/Components/PLScoreComponent.h"

APLPlayerState::APLPlayerState()
{
	PLScoreComponent = CreateDefaultSubobject<UPLScoreComponent>(FName(TEXT("PL Score Component")));
}

void APLPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

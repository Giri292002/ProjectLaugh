// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "PLPlayerController.h"

APLPlayerState::APLPlayerState()
{
	PLScoreComponent = CreateDefaultSubobject<UPLScoreComponent>(FName(TEXT("PL Score Component")));
}

void APLPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void APLPlayerState::Server_IncreaseConversion_Implementation()
{
	ConversionsThisRound += 1;
}

bool APLPlayerState::Server_IncreaseConversion_Validate()
{
	return true;
}

void APLPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APLPlayerState, ConversionsThisRound);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerState.h"

#include "Net/UnrealNetwork.h"
#include "PLPlayerController.h"

APLPlayerState::APLPlayerState()
{
	PLScoreComponent = CreateDefaultSubobject<UPLScoreComponent>(FName(TEXT("PL Score Component")));
	InitialCharacterUIData = nullptr;
}

void APLPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void APLPlayerState::PLReset_Implementation()
{
	ConversionsThisRound = 0;
	InitialCharacterUIData = nullptr;
}

void APLPlayerState::Server_SetCharacterUIProfileData_Implementation(UCharacterUIProfileData* InUIProfileData)
{
	CurrentCharacterUIData = InUIProfileData;
	if (InitialCharacterUIData == nullptr)
	{
		InitialCharacterUIData = InUIProfileData;
	}
}

bool APLPlayerState::Server_SetCharacterUIProfileData_Validate(UCharacterUIProfileData* InUIProfileData)
{
	return true;
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
	DOREPLIFETIME(APLPlayerState, CurrentCharacterUIData);
	DOREPLIFETIME(APLPlayerState, InitialCharacterUIData);
	DOREPLIFETIME(APLPlayerState, ElderCharacterClass);
}

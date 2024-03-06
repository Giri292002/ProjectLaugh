// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameState_Infection.h"

#include "Net/UnrealNetwork.h"
#include "PLGameMode_Infection.h"

void APLGameState_Infection::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APLGameState_Infection, CurrentRound);
	DOREPLIFETIME(APLGameState_Infection, CurrentBrainMeter);
	DOREPLIFETIME(APLGameState_Infection, MaxBrainMeter);
}

void APLGameState_Infection::RunBrainMeter(float StartingBrainMeter)
{
	CurrentBrainMeter = MaxBrainMeter = StartingBrainMeter;
	GetWorld()->GetTimerManager().SetTimer(BrainMeterTimer, this, &APLGameState_Infection::ReduceBrainMeter, 1.f, true);
}

void APLGameState_Infection::ReduceBrainMeter()
{
	CurrentBrainMeter -= 1.f;
	GEngine->AddOnScreenDebugMessage((uint64)("Timer"), 2.f, FColor::Green, FString::Printf(TEXT("Brain Meter: %f"), CurrentBrainMeter));
	OnBrainMeterUpdateDelegate.Broadcast(CurrentBrainMeter, MaxBrainMeter);
}

void APLGameState_Infection::OnRep_BrainMeter()
{
	OnBrainMeterUpdateDelegate.Broadcast(CurrentBrainMeter, MaxBrainMeter);
}

void APLGameState_Infection::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}



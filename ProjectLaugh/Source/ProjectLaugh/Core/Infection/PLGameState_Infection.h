// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Core/PLEOSGameState.h"
#include "PLGameState_Infection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBrainMeterSignature, float, CurrentBrainMeter, float, MaxBrainMeter);

UCLASS()
class PROJECTLAUGH_API APLGameState_Infection : public APLEOSGameState
{
	GENERATED_BODY()

protected:
	UPROPERTY(Replicated)
	int32 CurrentRound;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_BrainMeter)
	float CurrentBrainMeter;

	UPROPERTY(Replicated)
	float MaxBrainMeter;

	UPROPERTY()
	FTimerHandle BrainMeterTimer;

	UFUNCTION()
	void ReduceBrainMeter();

	UFUNCTION()
	void OnRep_BrainMeter();

public:
	UFUNCTION()
	void RunBrainMeter(float StartingBrainMeter);

	UFUNCTION()
	void IncreaseRound() { CurrentRound++; }	

	UPROPERTY()
	FBrainMeterSignature OnBrainMeterUpdateDelegate;

	virtual void Tick(float DeltaSeconds) override;
};

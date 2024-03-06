// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Core/PLEOSGameState.h"
#include "PLGameState_Infection.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBrainMeterSignature, float, CurrentBrainMeter, float, MaxBrainMeter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoundUpdateSignature, int, RoundNumber);

UCLASS()
class PROJECTLAUGH_API APLGameState_Infection : public APLEOSGameState
{
	GENERATED_BODY()

protected:
	APLGameState_Infection();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentRound)
	int32 CurrentRound;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_BrainMeter)
	float CurrentBrainMeter;

	UPROPERTY(Replicated)
	float MaxBrainMeter;

	UPROPERTY()
	FTimerHandle BrainMeterTimer;

	UPROPERTY()
	bool bIsRoundDone = false;

	UFUNCTION()
	void ReduceBrainMeter();

	UFUNCTION()
	void OnRep_BrainMeter();

	UFUNCTION()
	void OnRep_CurrentRound();

public:
	UFUNCTION()
	void RunBrainMeter(float StartingBrainMeter);

	UFUNCTION()
	void IncreaseRound();

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentRound() const { return CurrentRound; }

	//Calls EndRound if this is true
	UFUNCTION()
	bool CheckRoundWinCondition();

	UFUNCTION()
	void EndRound();

	UFUNCTION()
	void Server_SetDilation(float NewTimeDilation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetDilation(float NewTimeDilation);

	UPROPERTY()
	FBrainMeterSignature OnBrainMeterUpdateDelegate;

	UPROPERTY()
	FRoundUpdateSignature OnRoundUpdateDelegate;

	virtual void Tick(float DeltaSeconds) override;
};

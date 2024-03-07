// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Core/PLEOSGameState.h"
#include "ProjectLaugh/Core/System/PLResetInterface.h"
#include "ProjectLaugh/Core/Infection/PLInfectionGameModeData.h"
#include "ProjectLaugh/SharedGameplayTags.h"

#include "PLGameState_Infection.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBrainMeterSignature, float, CurrentBrainMeter, float, MaxBrainMeter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoundUpdateSignature, int, RoundNumber);


UCLASS()
class PROJECTLAUGH_API APLGameState_Infection : public APLEOSGameState, public IPLResetInterface
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

	UPROPERTY(Replicated)
	UPLInfectionGameModeData* InfectionGameModeData;

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
	void PrepareToEndRound(FGameplayTag InWinningTeam);

	UFUNCTION()
	void EndRound();

	UFUNCTION()
	void Server_SetDilation(float NewTimeDilation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetDilation(float NewTimeDilation);

	UFUNCTION()
	UPLInfectionGameModeData* GetGameData() const { return InfectionGameModeData; }

	UPROPERTY()
	FBrainMeterSignature OnBrainMeterUpdateDelegate;

	UPROPERTY()
	FRoundUpdateSignature OnRoundUpdateDelegate;

	UPROPERTY(Replicated)
	FGameplayTag WinningTeam;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void PLReset_Implementation() override;
};

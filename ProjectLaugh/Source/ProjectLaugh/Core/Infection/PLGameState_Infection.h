// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Core/PLEOSGameState.h"
#include "ProjectLaugh/Core/System/PLResetInterface.h"
#include "ProjectLaugh/Core/Infection/PLInfectionGameModeData.h"
#include "ProjectLaugh/SharedGameplayTags.h"

#include "PLGameState_Infection.generated.h"

class  UCharacterUIProfileData;
class APLPlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBrainMeterSignature, float, CurrentBrainMeter, float, MaxBrainMeter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoundUpdateSignature, int, RoundNumber);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterAddOrRemoveSignature);


UCLASS()
class PROJECTLAUGH_API APLGameState_Infection : public APLEOSGameState, public IPLResetInterface
{
	GENERATED_BODY()

protected:
	APLGameState_Infection();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_CurrentRound)
	int32 CurrentRound;

	//In Seconds
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_BrainMeter)
	float CurrentBrainMeter;

	//In Seconds
	UPROPERTY(Replicated)
	float MaxBrainMeter;

	UPROPERTY(Replicated)
	UPLInfectionGameModeData* InfectionGameModeData;

	UPROPERTY()
	FTimerHandle BrainMeterTimer;

	UPROPERTY()
	bool bIsRoundDone = false;

	UPROPERTY()
	bool bHasRoundStarted = false;

	UPROPERTY(Replicated)
	int32 NumberOfZombies = 0;

	UPROPERTY(Replicated)
	int32 NumberOfElders = 0;

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_InGameCharacters)
	TArray<APLPlayerCharacter*> InGameCharacters;

	UFUNCTION()
	void ReduceBrainMeter();

	UFUNCTION()
	void OnRep_BrainMeter();

	UFUNCTION()
	void OnRep_CurrentRound();

	UFUNCTION()
	void IncreaseElderCount() { NumberOfElders++; }

	UFUNCTION()
	void IncreaseZombieCount();

	UFUNCTION()
	void DecreaseElderCount() { NumberOfElders--; }

	UFUNCTION()
	void DecreaseZombieCount() { NumberOfZombies--; }

	//Handles adding character information to other players like UI
	UFUNCTION(BlueprintCallable)
	void RegisterCharacterToGame(FGameplayTag AffilitationTag, APLPlayerCharacter* NewCharacter);

	UFUNCTION()
	void OnRep_InGameCharacters();

public:
	UPROPERTY()
	FBrainMeterSignature OnBrainMeterUpdateDelegate;

	UPROPERTY()
	FRoundUpdateSignature OnRoundUpdateDelegate;

	UPROPERTY()
	FCharacterAddOrRemoveSignature OnCharacterAddOrRemoveSignature;

	UPROPERTY(Replicated)
	FGameplayTag WinningTeam;

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
	void StartRound();

	UFUNCTION()
	void EndRound();

	UFUNCTION()
	void Server_SetDilation(float NewTimeDilation);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetDilation(float NewTimeDilation);

	UFUNCTION()
	void RegisterElder(APLPlayerCharacter* NewCharacter);

	UFUNCTION()
	void RegisterZombie(APLPlayerCharacter* NewCharacter);

	UFUNCTION()
	void UnregisterElder(APLPlayerCharacter* CharacterToRemove);

	UFUNCTION()
	void UnregisterZombie(APLPlayerCharacter* CharacterToRemove);

	UFUNCTION(BlueprintCallable)
	void UnregisterCharacterFromGame(APLPlayerCharacter* NewCharacter);

	UFUNCTION()
	TArray<APLPlayerCharacter*> GetCharactersInGame() const { return InGameCharacters; }

	UFUNCTION()
	UPLInfectionGameModeData* GetGameData() const { return InfectionGameModeData; }

	UFUNCTION()
	float GetTimeSurvived();

	UFUNCTION()
	int GetNumberOfElders() const { return NumberOfElders; }

	UFUNCTION()
	int GetNumberOfZombies() const { return NumberOfZombies; }

	UFUNCTION()
	int GetTotalNumberOfPlayers() const { return GetNumberOfElders() + GetNumberOfZombies(); }

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void PLReset_Implementation() override;
};

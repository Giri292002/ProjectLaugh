// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/PLGameModeBase.h"
#include "ProjectLaugh/SharedGameplayTags.h"
#include "PLGameMode_Infection.generated.h"

class APLPlayerCharacter;
class APLPlayerCharacter_Elder;
class APLPlayerCharacter_Zombie;
class APLPlayerController;
class UPLInfectionGameModeData;
class APLGameState_Infection;

UCLASS()
class PROJECTLAUGH_API APLGameMode_Infection : public APLGameModeBase
{
	GENERATED_BODY()

	APLGameMode_Infection();

public:
	//Handles destroying pawn and spawning a new zombie and assigning it to controller
	UFUNCTION()
	void SpawnConvertedZombie(APLPlayerCharacter_Elder* Elder);

	virtual void SpawnPlayers() override;

	UPLInfectionGameModeData* GetGameData() const { return PLInfectionGameModeData; }

	UFUNCTION()
	void PrepareToStartRound();

	UFUNCTION()
	void StartRound();

	UFUNCTION()
	void PrepareToEndRound(FGameplayTag WinningTeam);

	UFUNCTION()
	void EndRound(FGameplayTag WinningTeam);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "PL | Infection")
	TArray<TSubclassOf<APLPlayerCharacter_Elder>>ElderClasses;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Infection")
	TArray<TSubclassOf<APLPlayerCharacter_Zombie>>ZombieClasses;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Infection")
	UPLInfectionGameModeData* PLInfectionGameModeData;

	UPROPERTY()
	APLGameState_Infection* PLGameState_Infection;

	virtual bool ReadyToStartMatch_Implementation() override;

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetMatchState(FName NewState) override;

	void SpawnPLPlayerCharacter(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController, FGameplayTag StartAffilitationTag);

	void SpawnPLPlayerCharacter(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController, FTransform& SpawnTransform);

	virtual void ResetLevel() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/PLGameModeBase.h"
#include "PLGameMode_Infection.generated.h"

class APLPlayerCharacter_Elder;
class APLPlayerCharacter_Zombie;
class APLPlayerController;

UCLASS()
class PROJECTLAUGH_API APLGameMode_Infection : public APLGameModeBase
{
	GENERATED_BODY()

	APLGameMode_Infection();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "PL | Infection")
	TArray<TSubclassOf<APLPlayerCharacter_Elder>>ElderClasses;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Infection")
	TArray<TSubclassOf<APLPlayerCharacter_Zombie>>ZombieClasses;

	virtual bool ReadyToStartMatch_Implementation() override;

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetMatchState(FName NewState) override;

	virtual void SpawnPlayers() override;

	UFUNCTION()
	void SpawnPLPlayerCharacter(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController);

};

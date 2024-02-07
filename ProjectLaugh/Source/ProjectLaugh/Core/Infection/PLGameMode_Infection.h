// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/PLGameModeBase.h"
#include "PLGameMode_Infection.generated.h"

class APLPlayerCharacter;

UCLASS()
class PROJECTLAUGH_API APLGameMode_Infection : public APLGameModeBase
{
	GENERATED_BODY()

	APLGameMode_Infection();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "ProjectLaugh | Infection")
	TArray<TSubclassOf<APLPlayerCharacter>>AllCharacterClasses;

	virtual bool ReadyToStartMatch_Implementation() override;

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetMatchState(FName NewState) override;

	virtual void SpawnPlayers() override;
};

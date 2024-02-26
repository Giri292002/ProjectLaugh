// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PLGameModeBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPLGameMode, Log, All)

class APLPlayerController;
class APLPlayerStart;

UCLASS()
class PROJECTLAUGH_API APLGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:
	APLGameModeBase();

public:
	UFUNCTION()
	bool GetSuitablePLPlayerStart(APLPlayerStart* &OutPLPLayerStart );

	virtual void PostLogin(APlayerController* NewPlayer) override;
	void PreLogout(APlayerController* InPlayerController);

	virtual void BeginPlay() override;

	virtual bool ReadyToStartMatch_Implementation() override;

	virtual void SetMatchState(FName NewState) override;

	virtual void SpawnPlayers();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "ProjectLaugh | Game Mode Defaults")
	TSubclassOf<APLPlayerStart> PLPlayerStartClass;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectLaugh | Players")
	int PlayersNeedToStartGame;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectLaugh | Players")
	TSet<APLPlayerController*> ConnectedPLPlayerControllers;

	UPROPERTY(BlueprintReadOnly, Category = "ProjectLaugh | Players")
	TArray<APLPlayerStart*> PLPlayerStarts;

	UFUNCTION()
	void InitializePLPlayerStarts();
};

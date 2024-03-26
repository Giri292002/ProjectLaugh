// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/PLGameModeBase.h"
#include "ProjectLaugh/Core/System/PLResetInterface.h"
#include "ProjectLaugh/SharedGameplayTags.h"
#include "PLGameMode_Infection.generated.h"

class APLPlayerCharacter;
class APLPlayerCharacter_Elder;
class APLPlayerCharacter_Zombie;
class APLPlayerController;
class UPLInfectionGameModeData;
class APLGameState_Infection;
class APLResultScreenPawn;
class APLWorldText;

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

	UFUNCTION()
	void DestroyPLPlayerCharacter(APLPlayerCharacter* CharacterToDestroy);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "PL | Infection")
	TArray<TSubclassOf<APLPlayerCharacter_Elder>>ElderClasses;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Infection")
	TArray<TSubclassOf<APLPlayerCharacter_Zombie>>AlphaZombieClasses;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Infection")
	TArray<TSubclassOf<APLPlayerCharacter_Zombie>>BetaZombieClasses;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Infection")
	TSubclassOf<APLResultScreenPawn> ResultsScreenPawnClass;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Infection")
	UPLInfectionGameModeData* PLInfectionGameModeData;

	UPROPERTY()
	APLGameState_Infection* PLGameState_Infection;

	virtual bool ReadyToStartMatch_Implementation() override;

	virtual void BeginPlay() override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetMatchState(FName NewState) override;

	UFUNCTION()
	void EndGame();

	UFUNCTION()
	APLWorldText* GetWorldText(TArray<APLWorldText*>& WorldTextActors, FGameplayTag Position);

	UFUNCTION()
	void AddZombieSpawnTimerToEveryone(const float ZombieSpawnTime);

	//Generic spawn pl player character, if you are trying to spawn elder or zombie, use SpawnZombie or SpawnElder instead
	APLPlayerCharacter* SpawnPLPlayerCharacter(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController, FGameplayTag StartAffilitationTag);

	APLPlayerCharacter* SpawnPLPlayerCharacter(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController, FTransform& SpawnTransform);


	//Handles correctly spawning a zombie and registering it
	void SpawnZombie(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController, bool bOverrideDefaultSpawnTransform = false, FTransform SpawnTransform = FTransform());

	void SpawnElder(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController, bool bOverrideDefaultSpawnTransform = false, FTransform SpawnTransform = FTransform());

	virtual void ResetLevel() override;
};

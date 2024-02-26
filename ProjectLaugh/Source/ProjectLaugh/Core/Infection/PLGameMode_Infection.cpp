// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameMode_Infection.h"

#include "ProjectLaugh/Core/PLEOSGameInstance.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Gameplay/PLPlayerStart.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Gameplay/Characters/PLPlayerCharacter_Elder.h"
#include "ProjectLaugh/Gameplay/Characters/PLPlayerCharacter_Zombie.h"

APLGameMode_Infection::APLGameMode_Infection()
{
	bStartPlayersAsSpectators = true;
	bUseSeamlessTravel = true;
	bPauseable = false;
	bDelayedStart = true;
}

bool APLGameMode_Infection::ReadyToStartMatch_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Num Player Currently: %d"), ConnectedPLPlayerControllers.Num());
	return ConnectedPLPlayerControllers.Num() >= PlayersNeedToStartGame;
}

void APLGameMode_Infection::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Running Begin play"));
	//Dont need to update players needed to start game if in editor, just use the number we provide
#if !WITH_EDITOR
	UPLEOSGameInstance* PLEOSGameInstance = GetGameInstance<UPLEOSGameInstance>();
	if (ensureAlways(PLEOSGameInstance))
	{
		UE_LOG(LogPLEOS, Log, TEXT("Updating playersNeedtostart game from game instance"));
		PlayersNeedToStartGame = PLEOSGameInstance->GetMaxPlayersInCurrentLobby();
	}
#endif
}

void APLGameMode_Infection::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APLPlayerController* PLPlayerController = Cast<APLPlayerController>(NewPlayer);

	if (!ensureAlwaysMsgf(PLPlayerController, TEXT("PLPlayerController is invalid")))
	{
		return;
	}

	//bDelayed start has been set, so we need to call StartMatch ourself
	if (ReadyToStartMatch())
	{
		StartMatch();
	}
	else
	{
		PLPlayerController->Client_DrawWaitingForPlayersWidget();
	}

}

void APLGameMode_Infection::Tick(float DeltaSeconds)
{
	//UE_LOG(LogTemp, Warning, TEXT("Match State: %s"), *(GetMatchState().ToString()))
	Super::Tick(DeltaSeconds);
}

void APLGameMode_Infection::SetMatchState(FName NewState)
{
	Super::SetMatchState(NewState);

	if (NewState == FName("InProgress"))
	{
		SpawnPlayers();
	}
}

void APLGameMode_Infection::SpawnPlayers()
{
	if (ElderClasses.Num() <= 0 || ZombieClasses.Num() <= 0)
	{
		UE_LOG(LogPLGameMode, Error, TEXT("There isnt enough player classes, check ElderClasses and ZomieClasses."));
		return;
	}

	TArray<APLPlayerController*> CurrentConnectedControllers = ConnectedPLPlayerControllers.Array();

	// Spawn Zombie First
	const int ZombiePlayerControllerIndex = FMath::RandRange(0, CurrentConnectedControllers.Num() - 1);
	const int ZombieCharacterClassIndex = FMath::RandRange(0, ZombieClasses.Num() - 1);
	SpawnPLPlayerCharacter(ZombieClasses[ZombieCharacterClassIndex], CurrentConnectedControllers[ZombiePlayerControllerIndex]);
	CurrentConnectedControllers.RemoveAt(ZombiePlayerControllerIndex);
	
	TArray<TSubclassOf<APLPlayerCharacter_Elder>> AvailableElderCharacterClasses = ElderClasses;
	for (APLPlayerController* PLPlayerController : CurrentConnectedControllers)
	{
		const int ElderPlayerControllerIndex = FMath::RandRange(0, CurrentConnectedControllers.Num() - 1);
		const int ElderCharacterClassIndex = FMath::RandRange(0, AvailableElderCharacterClasses.Num() - 1);
		SpawnPLPlayerCharacter(AvailableElderCharacterClasses[ElderCharacterClassIndex], CurrentConnectedControllers[ElderPlayerControllerIndex]);
		CurrentConnectedControllers.RemoveAt(ElderPlayerControllerIndex);
		AvailableElderCharacterClasses.RemoveAt(ElderCharacterClassIndex);
	}
}

void APLGameMode_Infection::SpawnPLPlayerCharacter(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController)
{
	OwningPlayerController->Client_RemoveWaitingForPlayersWidget();
	APLPlayerStart* PLPlayerStart;
	if (!GetSuitablePLPlayerStart(PLPlayerStart))
	{
		UE_LOG(LogPLGameMode, Error, TEXT("Cannot find suitable PL Player start"));
		return;
	}
	const FTransform SpawnTransform = PLPlayerStart->GetActorTransform();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APLPlayerCharacter* SpawnedCharacter = GetWorld()->SpawnActor<APLPlayerCharacter>(SpawningCharacterClass, SpawnTransform, SpawnParams);

	if (!ensureAlwaysMsgf(SpawnedCharacter, TEXT("Spawned PL Player character is invalid")))
	{
		return;
	}
	OwningPlayerController->Possess(Cast<APawn>(SpawnedCharacter));
}

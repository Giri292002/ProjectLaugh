// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameMode_Infection.h"

#include "ProjectLaugh/Core/PLEOSGameInstance.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Gameplay/PLPlayerStart.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"

APLGameMode_Infection::APLGameMode_Infection()
{
	bStartPlayersAsSpectators = true;
	bUseSeamlessTravel = true;
	bPauseable = false;
	bDelayedStart = true;

	//Dont need to update players needed to start game if in editor, just use the number we provide
#if !WITH_EDITOR
	UPLEOSGameInstance* PLEOSGameInstance = Cast<UPLEOSGameInstance>GetGameInstance()
	if (ensureAlways(PLEOSGameInstance))
	{
		UE_LOG(LogPLEOS, Log, TEXT("Updating playersNeedtostart game from game instance"));
		PlayersNeedToStartGame = PLEOSGameInstance->GetMaxPlayersInCurrentLobby();
	}	
#endif
}

bool APLGameMode_Infection::ReadyToStartMatch_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Num Player Currently: %d"), ConnectedPLPlayerControllers.Num());
	return ConnectedPLPlayerControllers.Num() >= PlayersNeedToStartGame;
}

void APLGameMode_Infection::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("Running Begin play"));
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
	//TODO: Build and test
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
	if (AllCharacterClasses.Num() <= 0)
	{
		UE_LOG(LogPLGameMode, Error, TEXT("AllCharacterClasses in empty."));
		return;
	}

	for (APLPlayerController* PLPlayerController : ConnectedPLPlayerControllers)
	{
		PLPlayerController->Client_RemoveWaitingForPlayersWidget();
		int RandomIndex = FMath::RandRange(0, AllCharacterClasses.Num() - 1);
		APLPlayerStart* PLPlayerStart;
		if (!GetSuitablePLPlayerStart(PLPlayerStart))
		{
			UE_LOG(LogPLGameMode, Error, TEXT("Cannot find suitable PL Player start"));
			break;
		}
		const FTransform SpawnTransform = PLPlayerStart->GetActorTransform();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		APLPlayerCharacter* SpawnedCharacter = GetWorld()->SpawnActor<APLPlayerCharacter>(AllCharacterClasses[RandomIndex].Get(), SpawnTransform, SpawnParams);

		if (!ensureAlwaysMsgf(SpawnedCharacter, TEXT("Spawned PL Player character is invalid")))
		{
			break;
		}
		PLPlayerController->Possess(Cast<APawn>(SpawnedCharacter));
	}
}

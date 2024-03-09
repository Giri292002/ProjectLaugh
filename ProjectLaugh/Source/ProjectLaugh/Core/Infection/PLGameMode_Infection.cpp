// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameMode_Infection.h"

#include "EngineUtils.h" 
#include "ProjectLaugh/Core/PLActor.h"
#include "ProjectLaugh/Core/PLStaticMeshActor.h"
#include "ProjectLaugh/Core/PLEOSGameInstance.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Core/Infection/PLInfectionGameModeData.h"
#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Core/System/PLResetInterface.h"
#include "ProjectLaugh/Data/PLPlayerAttributesData.h"
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

	PLGameState_Infection = GetGameState<APLGameState_Infection>();
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
		PrepareToStartRound();
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
	SpawnZombie(ZombieClasses[ZombieCharacterClassIndex], CurrentConnectedControllers[ZombiePlayerControllerIndex], true);
	CurrentConnectedControllers.RemoveAt(ZombiePlayerControllerIndex);
	
	TArray<TSubclassOf<APLPlayerCharacter_Elder>> AvailableElderCharacterClasses = ElderClasses;
	for (APLPlayerController* PLPlayerController : CurrentConnectedControllers)
	{
		const int ElderPlayerControllerIndex = FMath::RandRange(0, CurrentConnectedControllers.Num() - 1);
		const int ElderCharacterClassIndex = FMath::RandRange(0, AvailableElderCharacterClasses.Num() - 1);
		PLPlayerController->Client_RemoveWaitingForPlayersWidget();
		SpawnElder(AvailableElderCharacterClasses[ElderCharacterClassIndex], CurrentConnectedControllers[ElderPlayerControllerIndex]);
		CurrentConnectedControllers.RemoveAt(ElderPlayerControllerIndex);
		AvailableElderCharacterClasses.RemoveAt(ElderCharacterClassIndex);
	}
}

APLPlayerCharacter* APLGameMode_Infection::SpawnPLPlayerCharacter(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController, FGameplayTag StartAffilitationTag)
{
	APLPlayerStart* PLPlayerStart;
	if (!GetSuitablePLPlayerStart(PLPlayerStart, StartAffilitationTag))
	{
		UE_LOG(LogPLGameMode, Error, TEXT("Cannot find suitable PL Player start"));
		return nullptr;
	}
	FTransform SpawnTransform = PLPlayerStart->GetActorTransform();
	return SpawnPLPlayerCharacter(SpawningCharacterClass, OwningPlayerController, SpawnTransform);
}

APLPlayerCharacter* APLGameMode_Infection::SpawnPLPlayerCharacter(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController, FTransform& SpawnTransform)
{	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	APLPlayerCharacter* SpawnedCharacter = GetWorld()->SpawnActor<APLPlayerCharacter>(SpawningCharacterClass, SpawnTransform, SpawnParams);

	if (!ensureAlwaysMsgf(SpawnedCharacter, TEXT("Spawned PL Player character is invalid")))
	{
		return nullptr;
	}
	OwningPlayerController->Possess(Cast<APawn>(SpawnedCharacter));
	return SpawnedCharacter;
}

void APLGameMode_Infection::DestroyPLPlayerCharacter(APLPlayerCharacter* CharacterToDestroy)
{
	APLGameState_Infection* InfectionGameState = GetWorld()->GetGameState<APLGameState_Infection>();
	check(InfectionGameState);
	InfectionGameState->UnregisterElder(CharacterToDestroy);
	CharacterToDestroy->Destroy();
}

void APLGameMode_Infection::SpawnZombie(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController, bool bIsAlphaZombie, bool bOverrideDefaultSpawnTransform, FTransform SpawnTransform)
{
	APLPlayerCharacter* SpawnedCharacter = nullptr;
	if (!bOverrideDefaultSpawnTransform)
	{
		SpawnedCharacter = SpawnPLPlayerCharacter(SpawningCharacterClass, OwningPlayerController, SharedGameplayTags::TAG_Character_Affiliation_Zombie);
	}
	else
	{
		SpawnedCharacter = SpawnPLPlayerCharacter(SpawningCharacterClass, OwningPlayerController, SpawnTransform);
	}
	if (bIsAlphaZombie)
	{
		PLGameState_Infection->RegisterAlphaZombie(SpawnedCharacter);
	}
	else
	{
		PLGameState_Infection->RegisterZombie(SpawnedCharacter);
	}
}

void APLGameMode_Infection::SpawnElder(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController, bool bOverrideDefaultSpawnTransform, FTransform SpawnTransform)
{
	APLPlayerCharacter* SpawnedCharacter = nullptr;
	if (!bOverrideDefaultSpawnTransform)
	{
		SpawnedCharacter = SpawnPLPlayerCharacter(SpawningCharacterClass, OwningPlayerController, SharedGameplayTags::TAG_Character_Affiliation_Elder);
	}
	else
	{
		SpawnedCharacter = SpawnPLPlayerCharacter(SpawningCharacterClass, OwningPlayerController, SpawnTransform);
	}
	PLGameState_Infection->RegisterElder(SpawnedCharacter);
}

void APLGameMode_Infection::SpawnConvertedZombie(APLPlayerCharacter_Elder* Elder)
{
	APLPlayerController* PLPlayerController = Cast<APLPlayerController>(Elder->GetController());
	PLGameState_Infection->UnregisterCharacterFromGame(Elder);
	Elder->Net_ToggleFreezeCharacter(true);
	Elder->Net_OnPounced();
	Elder->Multicast_OnPounced();
	FTransform SpawnTransform = Elder->GetActorTransform();
	SpawnZombie(ZombieClasses[0], PLPlayerController, false, true, SpawnTransform);
}

void APLGameMode_Infection::PrepareToStartRound()
{
	PLGameState_Infection->IncreaseRound();

	auto CurrentConnectedControllers = ConnectedPLPlayerControllers;
	for (APLPlayerController* PLPlayerController : CurrentConnectedControllers)
	{
		PLPlayerController->Client_RemoveAllWidgets();
		PLPlayerController->Client_AddPLWidget(PLInfectionGameModeData->RoundWidget);
	}
	FTimerHandle StartToundTimerHandle;
	GetWorldTimerManager().SetTimer(StartToundTimerHandle, this, &APLGameMode_Infection::StartRound, PLInfectionGameModeData->PreRoundTime);
}

void APLGameMode_Infection::StartRound()
{
	SpawnPlayers();
	PLGameState_Infection->StartRound();
	PLGameState_Infection->RunBrainMeter(GetGameData()->BrainMeterTime * 60.f);
}

void APLGameMode_Infection::PrepareToEndRound(FGameplayTag WinningTeam)
{
	auto CurrentConnectedControllers = ConnectedPLPlayerControllers;
	for (APLPlayerController* PLPlayerController : CurrentConnectedControllers)
	{
		PLPlayerController->Client_AddPLWidget(PLInfectionGameModeData->RoundEndWidget);
	}
}

void APLGameMode_Infection::EndRound(FGameplayTag WinningTeam)
{
	auto CurrentConnectedControllers = ConnectedPLPlayerControllers;
	for (APLPlayerController* PLPlayerController : CurrentConnectedControllers)
	{
		APawn* Pawn = PLPlayerController->GetPawn();
		PLPlayerController->UnPossess();
		Pawn->Destroy();
		PLPlayerController->Client_RemoveAllWidgets();
		PLPlayerController->Client_AddPLWidget(PLInfectionGameModeData->ScoreWidget);
	}
	//TODO: Add Round Done, after round scoring stuff would go here
	ResetLevel();

	FTimerHandle StartRoundTimer;
	GetWorldTimerManager().SetTimer(StartRoundTimer, this, &APLGameMode_Infection::PrepareToStartRound, 5.f, false);
}

void APLGameMode_Infection::ResetLevel()
{
	//Reset all PLActors in level
	for (TActorIterator<APLActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		IPLResetInterface::Execute_PLReset(*ActorItr);
	}

	//Reset all PLStaticActors in level
	for (TActorIterator<APLStaticMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		IPLResetInterface::Execute_PLReset(*ActorItr);
	}

	//Reset Game State
	IPLResetInterface::Execute_PLReset(GetGameState<APLGameState_Infection>());
}


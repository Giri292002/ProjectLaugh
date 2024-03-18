// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameMode_Infection.h"

#include "EngineUtils.h" 
#include "Kismet/GameplayStatics.h"
#include "ProjectLaugh/Core/PLActor.h"
#include "ProjectLaugh/Core/PLStaticMeshActor.h"
#include "ProjectLaugh/Core/PLEOSGameInstance.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Core/PLPlayerState.h"
#include "ProjectLaugh/Core/Infection/PLInfectionGameModeData.h"
#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Components/PLScoreComponent.h"
#include "ProjectLaugh/Data/PLPlayerAttributesData.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Gameplay/Characters/PLResultScreenPawn.h"
#include "ProjectLaugh/Gameplay/Characters/PLPlayerCharacter_Elder.h"
#include "ProjectLaugh/Gameplay/Characters/PLPlayerCharacter_Zombie.h"
#include "ProjectLaugh/Gameplay/PLPlayerStart.h"
#include "ProjectLaugh/Widgets/Scores/PLScoreWidget.h"
#include <ProjectLaugh/PLWorldText.h>

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

	if (NewState == MatchState::InProgress)
	{
		PrepareToStartRound();
	}
	if (NewState == MatchState::WaitingPostMatch)
	{
		for (auto PLPlayerController : ConnectedPLPlayerControllers)
		{
			PLPlayerController->Client_OpenMainMenu(); 
		}
	}
}

void APLGameMode_Infection::SpawnPlayers()
{
	if (ElderClasses.Num() <= 0 || AlphaZombieClasses.Num() <= 0)
	{
		UE_LOG(LogPLGameMode, Error, TEXT("There isnt enough player classes, check ElderClasses and ZombieClasses."));
		return;
	}
	
	//Assign elder character classes and zombies randomly in first round
	if (GetGameState<APLGameState_Infection>()->GetCurrentRound() == 1)
	{
		TArray<APLPlayerController*> CurrentConnectedControllers = ConnectedPLPlayerControllers.Array();
		TArray<TSubclassOf<APLPlayerCharacter_Elder>> AvailableElderCharacterClasses = ElderClasses;
		//Pick a random player as zombie
		const int ZombiePlayerControllerIndex = FMath::RandRange(0, CurrentConnectedControllers.Num() - 1);
		APLPlayerController* ZombieController = CurrentConnectedControllers[ZombiePlayerControllerIndex];
		CurrentConnectedControllers.RemoveAt(ZombiePlayerControllerIndex);

		//Spawn Elders
		for (APLPlayerController* PLPlayerController : CurrentConnectedControllers)
		{
			const int ElderPlayerControllerIndex = FMath::RandRange(0, CurrentConnectedControllers.Num() - 1);
			const int ElderCharacterClassIndex = FMath::RandRange(0, AvailableElderCharacterClasses.Num() - 1);

			PLPlayerController->Client_RemoveWaitingForPlayersWidget();
			SpawnElder(AvailableElderCharacterClasses[ElderCharacterClassIndex], CurrentConnectedControllers[ElderPlayerControllerIndex]);
			CurrentConnectedControllers[ElderPlayerControllerIndex]->GetPlayerState<APLPlayerState>()->SetElderCharacterClass(AvailableElderCharacterClasses[ElderCharacterClassIndex]);
			CurrentConnectedControllers.RemoveAt(ElderPlayerControllerIndex);
			AvailableElderCharacterClasses.RemoveAt(ElderCharacterClassIndex);
		}

		//Spawn remaining controller as zombie	
		const int ZombieCharacterClassIndex = FMath::RandRange(0, AlphaZombieClasses.Num() - 1);
		SpawnZombie(AlphaZombieClasses[ZombieCharacterClassIndex], ZombieController);
		ZombieController->GetPlayerState<APLPlayerState>()->SetElderCharacterClass(AvailableElderCharacterClasses[0]);
	}
	else
	{
		TArray<APLPlayerController*> CurrentConnectedControllers = ConnectedPLPlayerControllers.Array();

		//Sort Controllers by score
		CurrentConnectedControllers.Sort([](const APLPlayerController& A, const APLPlayerController& B)
			{
				return A.PlayerState->GetScore() < B.PlayerState->GetScore();
			});

		//After sort, the first player will be the lowest score
		const int ZombiePlayerControllerIndex = 0;
		APLPlayerController* ZombieController = CurrentConnectedControllers[ZombiePlayerControllerIndex];
		CurrentConnectedControllers.RemoveAt(ZombiePlayerControllerIndex);

		//Spawn Elders
		for (APLPlayerController* PLPlayerController : CurrentConnectedControllers)
		{
			const int ElderPlayerControllerIndex = FMath::RandRange(0, CurrentConnectedControllers.Num() - 1);
			TSubclassOf<APLPlayerCharacter> ElderCharacterClass = CurrentConnectedControllers[ElderPlayerControllerIndex]->GetPlayerState<APLPlayerState>()->GetElderCharacterClass();

			PLPlayerController->Client_RemoveWaitingForPlayersWidget();
			SpawnElder(ElderCharacterClass, CurrentConnectedControllers[ElderPlayerControllerIndex]);
			CurrentConnectedControllers.RemoveAt(ElderPlayerControllerIndex);
		}

		//Spawn remaining controller as zombie	
		const int ZombieCharacterClassIndex = FMath::RandRange(0, AlphaZombieClasses.Num() - 1);
		SpawnZombie(AlphaZombieClasses[ZombieCharacterClassIndex], ZombieController);
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

void APLGameMode_Infection::SpawnZombie(TSubclassOf<APLPlayerCharacter> SpawningCharacterClass, APLPlayerController* OwningPlayerController, bool bOverrideDefaultSpawnTransform, FTransform SpawnTransform)
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

	if (SpawnedCharacter->GetGameplayTagComponent()->GetActiveGameplayTags().HasTag(SharedGameplayTags::TAG_Character_Affiliation_Zombie_Alpha))
	{
		AddZombieSpawnTimerToEveryone(SpawnedCharacter->GetPLPlayerAttributesData()->AppearanceTime);
	}
	PLGameState_Infection->RegisterZombie(SpawnedCharacter);
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
	SpawnZombie(BetaZombieClasses[0], PLPlayerController, true, SpawnTransform);
}

void APLGameMode_Infection::PrepareToStartRound()
{
	ResetLevel();

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
		APLPlayerCharacter* Pawn = Cast<APLPlayerCharacter>(PLPlayerController->GetPawn());
		PLPlayerController->UnPossess();
		Pawn->Server_Destroy();
		PLPlayerController->Client_RemoveAllWidgets();
		PLPlayerController->Client_AddPLWidget(PLInfectionGameModeData->ScoreWidget);
	}

	if (PLGameState_Infection->GetCurrentRound() + 1 > GetGameData()->NumberOfRounds)
	{
		//We have played all the rounds, end game
		EndGame();
		return;
	}
	else
	{
		FTimerHandle StartRoundTimer;
		GetWorldTimerManager().SetTimer(StartRoundTimer, this, &APLGameMode_Infection::PrepareToStartRound, GetGameData()->PostRoundTime, false);
	}
}

void APLGameMode_Infection::EndGame()
{
	check(ResultsScreenPawnClass);

	AActor* OutActor = UGameplayStatics::GetActorOfClass(GetWorld(), ResultsScreenPawnClass);
	check(OutActor);
	APLResultScreenPawn* ResultScreenPawn = Cast<APLResultScreenPawn>(OutActor);
	check(ResultScreenPawn);

	//Sort Controllers by score
	TArray<APLPlayerController*> CurrentConnectedControllers = ConnectedPLPlayerControllers.Array();
	CurrentConnectedControllers.Sort([](const APLPlayerController& A, const APLPlayerController& B)
		{
			return A.GetPlayerState<APLPlayerState>()->GetPLScoreComponent()->GetTotalScore() > B.GetPlayerState<APLPlayerState>()->GetPLScoreComponent()->GetTotalScore();
		});

	TArray<FGameplayTag> PositionTag = { SharedGameplayTags::TAG_Result_Position_01, SharedGameplayTags::TAG_Result_Position_02, SharedGameplayTags::TAG_Result_Position_03 };
	TArray<APLWorldText*> WorldTextActors;

	for (TActorIterator<APLWorldText> Iterator(GetWorld()); Iterator; ++Iterator)
	{
		WorldTextActors.Add(*Iterator);
	}

	//Spawn top 3 players
	for (int i = 0; i < 3; i++)
	{
		if (!CurrentConnectedControllers.IsValidIndex(i))
		{
			continue;
		}
		APLPlayerStart* PLPlayerStart;
		if (!GetSuitablePLPlayerStart(PLPlayerStart, PositionTag[i]))
		{
			checkf(PLPlayerStart, TEXT("Cannot find suitable player start. Have you created one in level?"));
		}

		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("%s %i"), *CurrentConnectedControllers[i]->GetPlayerState<APLPlayerState>()->GetPlayerName(), CurrentConnectedControllers[i]->GetPlayerState<APLPlayerState>()->GetPLScoreComponent()->GetTotalScore()));
		GetWorldText(WorldTextActors, PositionTag[i])->Multi_SetText(FText::FromString(FString::Printf(TEXT("%s \n %i"), *CurrentConnectedControllers[i]->GetPlayerState<APLPlayerState>()->GetPlayerName(), CurrentConnectedControllers[i]->GetPlayerState<APLPlayerState>()->GetPLScoreComponent()->GetTotalScore())));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		APLPlayerCharacter* SpawnedCharacter = GetWorld()->SpawnActor<APLPlayerCharacter>(CurrentConnectedControllers[i]->GetPlayerState<APLPlayerState>()->GetElderCharacterClass(), PLPlayerStart->GetActorTransform(), SpawnParams);
	}

	for (APLPlayerController* PLPlayerController : CurrentConnectedControllers)
	{
		PLPlayerController->Client_RemoveAllWidgets();
		PLPlayerController->Client_PlayResultCinematicSequence();
	}
	FTimerHandle KickPlayersHandle;
	GetWorld()->GetTimerManager().SetTimer(KickPlayersHandle, this, &APLGameMode_Infection::EndMatch, 10.f);
}

APLWorldText* APLGameMode_Infection::GetWorldText(TArray<APLWorldText*>& WorldTextActors, FGameplayTag Position)
{
	for (auto WorldTextActor : WorldTextActors)
	{
		if (WorldTextActor->GetTextTag() == Position)
		{
			return WorldTextActor;
		}
	}
	return nullptr;
}

void APLGameMode_Infection::AddZombieSpawnTimerToEveryone(const float ZombieSpawnTime)
{
	for (APLPlayerController* PLPlayerController : ConnectedPLPlayerControllers)
	{
		PLPlayerController->Client_AddTimer(ZombieSpawnTime, FText::FromString("Alpha Zombie Spawning..."), false);
	}
}

void APLGameMode_Infection::ResetLevel()
{
	ExecutePLReset<APLActor>();
	ExecutePLReset<APLStaticMeshActor>();
	ExecutePLReset<APLPlayerStart>();
	ExecutePLReset<APLPlayerState>();

	//Reset Game State
	IPLResetInterface::Execute_PLReset(GetGameState<APLGameState_Infection>());
}


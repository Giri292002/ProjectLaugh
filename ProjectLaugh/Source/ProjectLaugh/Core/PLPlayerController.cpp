// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "ProjectLaugh/PLGameModeBase.h"
#include "ProjectLaugh/Widgets/PLWaitingForPlayersWidget.h"
#include "InputMappingContext.h"

void APLPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}

void APLPlayerController::Client_DrawWaitingForPlayersWidget_Implementation()
{
	if (!IsLocalPlayerController())
	{
		return;
	}
	if (!ensureAlwaysMsgf(PLWaitingForPlayersWidgetClass, TEXT("PLWaitingForPlayersWidget class is invalid")))
	{
		return;
	}

	PlayWaitingCinematicSequence();
	
	PLWaitingForPlayersWidget = CreateWidget<UPLWaitingForPlayersWidget>(GetWorld(), PLWaitingForPlayersWidgetClass);
	PLWaitingForPlayersWidget->AddToViewport();	
}

void APLPlayerController::PlayWaitingCinematicSequence()
{
	if (ensureAlwaysMsgf(WaitingSequence, TEXT("Waiting Sequence is invalid")))
	{
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		PlaybackSettings.bAutoPlay = true;
		FMovieSceneSequenceLoopCount LoopCount;
		LoopCount.Value = -1;
		PlaybackSettings.LoopCount = LoopCount;
		
		ALevelSequenceActor* OutActor;
		LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), WaitingSequence, PlaybackSettings, OutActor);
		LevelSequencePlayer->Play();
	}
}

void APLPlayerController::StoPlayingWaitingCinematicSequence()
{
	if (IsValid(LevelSequencePlayer))
	{
		LevelSequencePlayer->Stop();
	}
}

void APLPlayerController::Client_RemoveWaitingForPlayersWidget_Implementation()
{
	if (!IsLocalPlayerController())
	{
		return;
	}

	StoPlayingWaitingCinematicSequence();

	if (PLWaitingForPlayersWidget)
	{
		PLWaitingForPlayersWidget->RemoveFromParent();
		PLWaitingForPlayersWidget = nullptr;
	}
}

void APLPlayerController::OnNetCleanup(UNetConnection* Connection)
{
	if (GetLocalRole() == ROLE_Authority && PlayerState != NULL)
	{
		APLGameModeBase* GameMode = Cast<APLGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->PreLogout(this);
		}
	}

	Super::OnNetCleanup(Connection);
}

void APLPlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);

	if (!ensureAlwaysMsgf(DefaultMappingContext, TEXT("DefaultMappingContext is invalid")))
	{
		return;
	}
	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		
	}
}


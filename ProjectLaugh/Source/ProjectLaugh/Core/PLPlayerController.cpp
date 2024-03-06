// Fill out your copyright notice in the Description page of Project Settings.


#include "PLPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "Net/UnrealNetwork.h"
#include "ProjectLaugh/PLGameModeBase.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Components/PLActorComponent.h"
#include "ProjectLaugh/Gameplay/PLInhalerComponent.h"
#include "ProjectLaugh/Gameplay/Interaction/PLInteractionComponent.h"
#include "ProjectLaugh/Widgets/PLComponentWidgetBase.h"
#include "ProjectLaugh/Widgets/PLGameplayWidget.h"
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

	checkf(PLWaitingForPlayersWidgetClass, TEXT("PLWaitingForPlayersWidget class is invalid"));

	PlayWaitingCinematicSequence();

	PLWaitingForPlayersWidget = Internal_AddWidget<UPLWaitingForPlayersWidget>(PLWaitingForPlayersWidgetClass);
}

void APLPlayerController::Client_AddComponentWidgets_Implementation()
{
	if (!IsValid(GetPawn()))
	{
		return;
	}
	APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(GetPawn());
	TArray<UPLActorComponent*> PLActorComponents;
	PLPlayerCharacter->GetComponents<UPLActorComponent>(PLActorComponents);

	if (!PLActorComponents.Num())
	{
		return;
	}

	//Iterate through all the components, if they have a widget, create, add and send the widget pointer back to the component
	for (UPLActorComponent* PLActorComp : PLActorComponents)
	{
		if (IsValid(PLActorComp->GetComponentWidgetClass()))
		{
			UPLComponentWidgetBase* SpawnedComponentWidget = CreateWidget<UPLComponentWidgetBase>(GetWorld(), PLActorComp->GetComponentWidgetClass());
			Client_AddComponentWidget(PLActorComp->GetComponentWidgetClass(), PLActorComp); 
		}		
	}
}

void APLPlayerController::Client_RemoveComponentWidgets_Implementation()
{
	APLPlayerCharacter* PLPlayerCharacter = Cast<APLPlayerCharacter>(GetPawn());
	TArray<UPLActorComponent*> PLActorComponents;
	PLPlayerCharacter->GetComponents<UPLActorComponent>(PLActorComponents);

	if (!PLActorComponents.Num())
	{
		return;
	}

	for (UPLActorComponent* PLActorComp : PLActorComponents)
	{
		if (IsValid(PLActorComp->GetSpawnedComponentWidget()))
		{
			PLActorComp->GetSpawnedComponentWidget()->RemoveFromParent();
		}
	}
}

void APLPlayerController::Client_AddTimer_Implementation(float InSeconds, const FText& InTimerText, bool InbForward)
{
	if (IsValid(PLGameplayWidget))
	{
		PLGameplayWidget->AddTimer(InSeconds, InTimerText, InbForward);
	}
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
	Client_RemoveWaitingForPlayersWidget();

	SetViewTarget(NewPawn);

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
	Client_DrawGameplayWidget();
	Client_AddComponentWidgets();
}

void APLPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		RepPlayerControllerRotation = GetControlRotation();
	}
}

void APLPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APLPlayerController, RepPlayerControllerRotation);
}

void APLPlayerController::Client_AddPLWidget_Implementation(TSubclassOf<UPLWidgetBase> WidgetClassToAdd)
{
	Internal_AddWidget<UPLWidgetBase>(WidgetClassToAdd);
}

void APLPlayerController::Client_AddComponentWidget_Implementation(TSubclassOf<UPLComponentWidgetBase> WidgetClassToAdd, UPLActorComponent* InComp)
{
	auto SpawnedWidget = Internal_AddWidget<UPLComponentWidgetBase>(WidgetClassToAdd);
	SpawnedWidget->SetupComponent(InComp);
	InComp->SetSpawnedComponentWidget(SpawnedWidget);
}

template<typename T>
inline T* APLPlayerController::Internal_AddWidget(TSubclassOf<T> WidgetClassToAdd)
{
	T* CreatedWidget = CreateWidget<T>(GetWorld(), WidgetClassToAdd);
	CreatedWidget->AddToViewport();
	return CreatedWidget;
}

void APLPlayerController::Client_DrawGameplayWidget_Implementation()
{
	//Spawn a gameplay widget if there isn't already one
	if (!IsValid(PLGameplayWidget))
	{
		PLGameplayWidget = Internal_AddWidget<UPLGameplayWidget>(PLGameplayWidgetClass);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameModeBase.h"

#include "Core/PLEOSGameInstance.h"
#include "GameFramework/GameSession.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSubsystem.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSessionSettings.h"
#include <OnlineSubsystem.h>
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "ProjectLaugh/Gameplay/PLPlayerStart.h"

DEFINE_LOG_CATEGORY(LogPLGameMode)

APLGameModeBase::APLGameModeBase()
{
}

void APLGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void APLGameModeBase::InitializePLPlayerStarts()
{
	if (!ensureAlwaysMsgf(PLPlayerStartClass, TEXT("PL Player start class is null")))
	{
		return;
	}
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PLPlayerStartClass, OutActors);
	if (OutActors.Num() <= 0)
	{
		UE_LOG(LogPLGameMode, Error, TEXT("Cannot find any actors of typer PL Player Start"));
		return;
	}

	for (AActor* PlayerStart : OutActors)
	{
		APLPlayerStart* PLPlayerStart = Cast<APLPlayerStart>(PlayerStart);
		if (ensureAlwaysMsgf(PLPlayerStart, TEXT("PLPLayerStart is invalid")))
		{
			PLPlayerStarts.Add(PLPlayerStart);
		}
	}
}

bool APLGameModeBase::GetSuitablePLPlayerStart(APLPlayerStart* &OutPLPLayerStart, FGameplayTag StartAffilitationTag)
{
	if (PLPlayerStarts.Num() <= 0)
	{
		UE_LOG(LogPLGameMode, Error, TEXT("PL Player Start is empty. Did InitializePLPlayerStart() run?"));
		return false;
	}
	for (APLPlayerStart* PLPlayerStart : PLPlayerStarts)
	{
		if (PLPlayerStart->HasBeenUsed() || StartAffilitationTag != PLPlayerStart->StartAffiliationTag)
		{
			continue;
		}
		PLPlayerStart->SetHasBeenUsed(true);
		OutPLPLayerStart = PLPlayerStart;
		return true;
	}
	return false;
}

void APLGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	//Need to register player so EOS knows that its a valid player and doesn't kick them
	Super::PostLogin(NewPlayer);

	APLPlayerController* PLPlayerController = Cast<APLPlayerController>(NewPlayer);
	if (ensureAlwaysMsgf(PLPlayerController, TEXT("PLPlayerController is invalid")))
	{
		ConnectedPLPlayerControllers.Add(PLPlayerController);
	}

#if !WITH_EDITOR
	check(IsValid(NewPlayer));
	
	FUniqueNetIdRepl UniqueNetIDRepl;

	if (NewPlayer->IsLocalController())
	{
		ULocalPlayer* LocalPlayerRef = NewPlayer->GetLocalPlayer();
		if (LocalPlayerRef)
		{
			UniqueNetIDRepl = LocalPlayerRef->GetPreferredUniqueNetId();
		}
		else
		{
			UNetConnection* RemoteNetConnection = Cast<UNetConnection>(NewPlayer->Player);
			check(IsValid(RemoteNetConnection));
			UniqueNetIDRepl = RemoteNetConnection->PlayerId;
		}
	}
	else
	{
		UNetConnection* RemoteNetConnection = Cast<UNetConnection>(NewPlayer->Player);
		check(IsValid(RemoteNetConnection));
		UniqueNetIDRepl = RemoteNetConnection->PlayerId;
	}

	TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIDRepl.GetUniqueNetId();
	check(UniqueNetId != nullptr);

	// Get the online session interface.
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (ensure(OnlineSubsystem))
	{
		if (IOnlineSessionPtr SessionPtr = OnlineSubsystem->GetSessionInterface())
		{
			if (!SessionPtr->RegisterPlayer(PLSessionName, *UniqueNetId, false))
			{
				UE_LOG(LogTemp, Error, TEXT("Kicking Player LMAO"));
				GameSession->KickPlayer(NewPlayer,FText::FromString("Could not register"));
			}
		}
	}
#endif

	
}



bool APLGameModeBase::ReadyToStartMatch_Implementation()
{
	return Super::ReadyToStartMatch_Implementation();
}

void APLGameModeBase::SetMatchState(FName NewState)
{
	Super::SetMatchState(NewState);

	if (NewState == "WaitingToStart")
	{
		InitializePLPlayerStarts();
	}
}

void APLGameModeBase::SpawnPlayers()
{
	UE_LOG(LogPLGameMode, Error, TEXT("Please implement spawn players logic"));
}



void APLGameModeBase::PreLogout(APlayerController* InPlayerController)
{
	check(IsValid(InPlayerController));

	// This code handles logins for both the local player (listen server) and remote players (net connection).
	FUniqueNetIdRepl UniqueNetIdRepl;
	if (InPlayerController->IsLocalPlayerController())
	{
		ULocalPlayer* LocalPlayer = InPlayerController->GetLocalPlayer();
		if (IsValid(LocalPlayer))
		{
			UniqueNetIdRepl = LocalPlayer->GetPreferredUniqueNetId();
		}
		else
		{
			UNetConnection* RemoteNetConnection = Cast<UNetConnection>(InPlayerController->Player);
			check(IsValid(RemoteNetConnection));
			UniqueNetIdRepl = RemoteNetConnection->PlayerId;
		}
	}
	else
	{
		UNetConnection* RemoteNetConnection = Cast<UNetConnection>(InPlayerController->Player);
		check(IsValid(RemoteNetConnection));
		UniqueNetIdRepl = RemoteNetConnection->PlayerId;
	}

	// Get the unique player ID.
	TSharedPtr<const FUniqueNetId> UniqueNetId = UniqueNetIdRepl.GetUniqueNetId();
	check(UniqueNetId != nullptr);

	// Get the online session interface.
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	IOnlineSessionPtr Session = Subsystem->GetSessionInterface();

	if (!Session->UnregisterPlayer(PLSessionName, *UniqueNetId))
	{
		UE_LOG(LogPLGameMode, Error, TEXT("Cannot Register Player"));
	}
}
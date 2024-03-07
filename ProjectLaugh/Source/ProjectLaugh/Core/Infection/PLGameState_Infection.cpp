// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameState_Infection.h"
#include "PLGameMode_Infection.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PLGameMode_Infection.h"

void APLGameState_Infection::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APLGameState_Infection, CurrentRound);
	DOREPLIFETIME(APLGameState_Infection, CurrentBrainMeter);
	DOREPLIFETIME(APLGameState_Infection, MaxBrainMeter);
	DOREPLIFETIME(APLGameState_Infection, WinningTeam);
	DOREPLIFETIME(APLGameState_Infection, InfectionGameModeData);
}

void APLGameState_Infection::RunBrainMeter(float StartingBrainMeter)
{
	CurrentBrainMeter = MaxBrainMeter = StartingBrainMeter;
	GetWorld()->GetTimerManager().SetTimer(BrainMeterTimer, this, &APLGameState_Infection::ReduceBrainMeter, 1.f, true);
}

void APLGameState_Infection::IncreaseRound()
{
	CurrentRound += 1;
	GEngine->AddOnScreenDebugMessage((uint64)("Round"), 10.f, FColor::Green, FString::Printf(TEXT("Round: %i"), CurrentRound));
	OnRoundUpdateDelegate.Broadcast(CurrentRound);
}

bool APLGameState_Infection::CheckRoundWinCondition()
{
	//TODO: Add More win conditions
	if (bIsRoundDone)
	{
		return true;
	}

	if (CurrentBrainMeter <= 0.f)
	{
		PrepareToEndRound(SharedGameplayTags::TAG_Character_Affiliation_Elder);
		bIsRoundDone = true;
	}
	return bIsRoundDone;
}

void APLGameState_Infection::PrepareToEndRound(FGameplayTag InWinningTeam)
{
	GEngine->AddOnScreenDebugMessage((uint64)("RoundEnd"), 10.f, FColor::Red, FString::Printf(TEXT("Round End"), CurrentRound));
	GetWorldTimerManager().ClearTimer(BrainMeterTimer);
	WinningTeam = InWinningTeam;
	APLGameMode_Infection* InfectionGameMode = Cast<APLGameMode_Infection>(AuthorityGameMode);
	InfectionGameMode->PrepareToEndRound(InWinningTeam);
	FTimerHandle EndRoundTimer;
	GetWorld()->GetTimerManager().SetTimer(EndRoundTimer, this, &APLGameState_Infection::EndRound, 3.f, false);
	Multicast_SetDilation(0.5f);
}

void APLGameState_Infection::EndRound()
{
	Multicast_SetDilation(1.f);
	APLGameMode_Infection* InfectionGameMode = Cast<APLGameMode_Infection>(AuthorityGameMode);
	InfectionGameMode->EndRound(WinningTeam);
}

void APLGameState_Infection::Server_SetDilation(float NewTimeDilation)
{
	Multicast_SetDilation(NewTimeDilation);
}

void APLGameState_Infection::Multicast_SetDilation_Implementation(float NewTimeDilation)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), NewTimeDilation);
}

APLGameState_Infection::APLGameState_Infection()
{
	CurrentRound = 0;
}

void APLGameState_Infection::ReduceBrainMeter()
{
	CurrentBrainMeter = FMath::Clamp(CurrentBrainMeter -= 1.f, 0.f, MaxBrainMeter);
	OnBrainMeterUpdateDelegate.Broadcast(CurrentBrainMeter, MaxBrainMeter);
	CheckRoundWinCondition();
}

void APLGameState_Infection::OnRep_BrainMeter()
{
	OnBrainMeterUpdateDelegate.Broadcast(CurrentBrainMeter, MaxBrainMeter);
}

void APLGameState_Infection::OnRep_CurrentRound()
{
	OnRoundUpdateDelegate.Broadcast(CurrentRound);
}

void APLGameState_Infection::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		APLGameMode_Infection* InfectionGM = Cast<APLGameMode_Infection>(AuthorityGameMode);
		InfectionGameModeData = InfectionGM->GetGameData();
	}
}

void APLGameState_Infection::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APLGameState_Infection::PLReset_Implementation()
{
	bIsRoundDone = false;
	WinningTeam = FGameplayTag();
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameState_Infection.h"

#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PLGameMode_Infection.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "ProjectLaugh/Core/Infection/PLInfectionGameModeData.h"
#include "ProjectLaugh/Core/PLPlayerState.h"
#include "ProjectLaugh/Gameplay/Characters/PLPlayerCharacter_Zombie.h"
#include "ProjectLaugh/Data/PLPlayerAttributesData.h"

void APLGameState_Infection::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APLGameState_Infection, CurrentRound);
	DOREPLIFETIME(APLGameState_Infection, CurrentBrainMeter);
	DOREPLIFETIME(APLGameState_Infection, MaxBrainMeter);
	DOREPLIFETIME(APLGameState_Infection, WinningTeam);
	DOREPLIFETIME(APLGameState_Infection, InfectionGameModeData);
	DOREPLIFETIME(APLGameState_Infection, NumberOfElders);
	DOREPLIFETIME(APLGameState_Infection, NumberOfZombies);
	DOREPLIFETIME(APLGameState_Infection, InGameCharacters);
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
	if (!bHasRoundStarted)
	{
		return false;
	}

	if (bIsRoundDone)
	{
		return true;
	}

	if (CurrentBrainMeter <= 0.f)
	{
		PrepareToEndRound(SharedGameplayTags::TAG_Character_Affiliation_Elder);
		bIsRoundDone = true;
	}
	if (NumberOfZombies >= PlayerArray.Num())
	{
		PrepareToEndRound(SharedGameplayTags::TAG_Character_Affiliation_Zombie);
		bIsRoundDone = true;
	}
	return bIsRoundDone;
}

void APLGameState_Infection::PrepareToEndRound(FGameplayTag InWinningTeam)
{
	GetWorldTimerManager().ClearTimer(BrainMeterTimer);
	WinningTeam = InWinningTeam;

	for (APLPlayerCharacter* PLPlayerCharacter : InGameCharacters)
	{
		const FGameplayTagContainer TagContainer = PLPlayerCharacter->GetGameplayTagComponent()->GetActiveGameplayTags();
		//No need to give scores to beta zombies if the winners are zombies
		if (WinningTeam == SharedGameplayTags::TAG_Character_Affiliation_Zombie && !(TagContainer.HasTag(SharedGameplayTags::TAG_Character_Affiliation_Zombie_Alpha)))
		{
			continue;
		}

		if (WinningTeam == SharedGameplayTags::TAG_Character_Affiliation_Elder)
		{
			//Register time survived for surviving elders
			if (TagContainer.HasTag(WinningTeam))
			{
				PLPlayerCharacter->GetPlayerState<APLPlayerState>()->GetPLScoreComponent()->Server_AddScoreFromTimeSurvived();
				PLPlayerCharacter->GetPlayerState<APLPlayerState>()->GetPLScoreComponent()->Server_AddScoreFromPositionSurvived();
			}
		}

		if(PLPlayerCharacter->GetGameplayTagComponent()->GetActiveGameplayTags().HasTagExact(InWinningTeam))
		{
			PLPlayerCharacter->GetPlayerState<APLPlayerState>()->GetPLScoreComponent()->Server_AddScoreForWinningTeam(InWinningTeam);
		}
	}

	APLGameMode_Infection* InfectionGameMode = Cast<APLGameMode_Infection>(AuthorityGameMode);
	InfectionGameMode->PrepareToEndRound(InWinningTeam);
	FTimerHandle EndRoundTimer;
	GetWorld()->GetTimerManager().SetTimer(EndRoundTimer, this, &APLGameState_Infection::EndRound, 3.f, false);
	Multicast_SetDilation(0.5f);
}

void APLGameState_Infection::StartRound()
{
	bHasRoundStarted = true;
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

void APLGameState_Infection::RegisterElder(APLPlayerCharacter* NewCharacter)
{
	IncreaseElderCount();
	RegisterCharacterToGame(SharedGameplayTags::TAG_Character_Affiliation_Elder, NewCharacter);
}

void APLGameState_Infection::RegisterZombie(APLPlayerCharacter* NewCharacter)
{
	IncreaseZombieCount();
	RegisterCharacterToGame(SharedGameplayTags::TAG_Character_Affiliation_Zombie, NewCharacter);
}

void APLGameState_Infection::RegisterAlphaZombie(APLPlayerCharacter* NewCharacter)
{
	check(NewCharacter);
	NewCharacter->GetGameplayTagComponent()->Server_AddTag(SharedGameplayTags::TAG_Character_Affiliation_Zombie_Alpha);
	AlphaZombie = Cast<APLPlayerCharacter_Zombie>(NewCharacter);
	RegisterZombie(NewCharacter);
}

void APLGameState_Infection::UnregisterElder(APLPlayerCharacter* CharacterToRemove)
{
	DecreaseElderCount();
	UnregisterCharacterFromGame(CharacterToRemove); 
}

void APLGameState_Infection::UnregisterZombie(APLPlayerCharacter* CharacterToRemove)
{
	DecreaseZombieCount();
	UnregisterCharacterFromGame(CharacterToRemove);
}

void APLGameState_Infection::IncreaseZombieCount()
{
	NumberOfZombies++;
	CheckRoundWinCondition();
}

void APLGameState_Infection::RegisterCharacterToGame(FGameplayTag AffilitationTag, APLPlayerCharacter* NewCharacter)
{
	if (!IsValid(NewCharacter))
	{
		return;
	}

	InGameCharacters.Add(NewCharacter);
	NewCharacter->GetPLPlayerController()->GetPlayerState<APLPlayerState>()->Server_SetCharacterUIProfileData(NewCharacter->GetCharacterUIData());
	OnCharacterAddOrRemoveSignature.Broadcast();
}

void APLGameState_Infection::OnRep_InGameCharacters()
{
	OnCharacterAddOrRemoveSignature.Broadcast();
}

void APLGameState_Infection::UnregisterCharacterFromGame(APLPlayerCharacter* NewCharacter)
{
	InGameCharacters.Remove(NewCharacter);
	OnCharacterAddOrRemoveSignature.Broadcast();
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

float APLGameState_Infection::GetTimeSurvived()
{
	return MaxBrainMeter - CurrentBrainMeter;
}

void APLGameState_Infection::GiveAlphaZombieAssist()
{
	if (IsValid(GetAlphaZombie()->GetPLPlayerController()))
	{
		APLPlayerState* PLPlayerState = GetAlphaZombie()->GetPLPlayerController()->GetPlayerState<APLPlayerState>();
		if (IsValid(PLPlayerState))
		{
			PLPlayerState->GetPLScoreComponent()->Server_AddScoreFromConversionAssist();
		}
	}
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
	NumberOfElders = 0;
	NumberOfZombies = 0;
	bHasRoundStarted = false;
	InGameCharacters.Empty();
}



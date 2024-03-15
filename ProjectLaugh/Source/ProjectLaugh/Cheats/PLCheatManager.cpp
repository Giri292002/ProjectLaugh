// Fill out your copyright notice in the Description page of Project Settings.


#include "PLCheatManager.h"

#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"

void UPLCheatManager::EndRound(FString WinningTeam)
{
	FGameplayTag WinningTag;
	if (WinningTeam == "z" || WinningTeam == "zombie" || WinningTeam == "Zombie")
	{
		WinningTag = SharedGameplayTags::TAG_Character_Affiliation_Zombie;
	}
	else if(WinningTeam == "e" || WinningTeam == "elder" || WinningTeam == "Elder")
	{
		WinningTag = SharedGameplayTags::TAG_Character_Affiliation_Elder;
	}

	APLGameState_Infection* InfectionGameState = GetWorld()->GetGameState<APLGameState_Infection>();
	check(InfectionGameState);
	InfectionGameState->PrepareToEndRound(WinningTag);
}

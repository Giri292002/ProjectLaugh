// Fill out your copyright notice in the Description page of Project Settings.


#include "PLRoundEndWidget.h"

#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "Components/TextBlock.h"

void UPLRoundEndWidget::PLConstruct()
{
	APLGameState_Infection* InfectionGameState = Cast<APLGameState_Infection>(PLGameState);
	FGameplayTag WinningTeam = InfectionGameState->WinningTeam;
	FText VictoryText = WinningTeam == SharedGameplayTags::TAG_Character_Affiliation_Zombie ? FText::FromString("ZOMBIES WON") : FText::FromString("ELDERS WON");
	VictoryTextBlock->SetText(VictoryText);
	PlayAnimation(VictoryAnimation);
}

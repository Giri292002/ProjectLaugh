// Fill out your copyright notice in the Description page of Project Settings.


#include "PLRoundEndWidget.h"

#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Core/PLPlayerController.h"
#include "Components/TextBlock.h"

void UPLRoundEndWidget::PLConstruct()
{
	APLGameState_Infection* InfectionGameState = Cast<APLGameState_Infection>(PLGameState);
	FGameplayTag WinningTeam = InfectionGameState->WinningTeam;
	if (APLPlayerController* PLPlayerController = Cast<APLPlayerController>(GetOwningPlayer()))
	{
		FGameplayTag MyTeam = PLPlayerController->GetCurrentAffiliationTag();
		FText VictoryText = WinningTeam == MyTeam ? FText::FromString("ROUND WON") : FText::FromString("ROUND LOST");
		VictoryTextBlock->SetText(VictoryText);
	}
	PlayAnimation(VictoryAnimation);
}

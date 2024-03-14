// Fill out your copyright notice in the Description page of Project Settings.


#include "PLBigRoundWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Core/Infection/PLInfectionGameModeData.h"

void UPLBigRoundWidget::PLConstruct()
{
	APLGameState_Infection* InfectionGameState = Cast<APLGameState_Infection>(PLGameState);
	RoundText->SetText(FText::FromString(FString::FromInt(InfectionGameState->GetCurrentRound())));
	FTimerHandle RemoveWidgetTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(RemoveWidgetTimerHandle, this, &UPLBigRoundWidget::RemoveFromParent, InfectionGameState->GetGameData()->PreRoundTime, false);
}

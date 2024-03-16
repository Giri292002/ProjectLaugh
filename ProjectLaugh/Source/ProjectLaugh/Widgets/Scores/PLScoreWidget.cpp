// Fill out your copyright notice in the Description page of Project Settings.


#include "PLScoreWidget.h"

#include "Components/VerticalBox.h"
#include "GameFramework/PlayerState.h"
#include "ProjectLaugh/Core/Infection/PLInfectionGameModeData.h"
#include "ProjectLaugh/Core/PLPlayerState.h"
#include "ProjectLaugh/Widgets/Scores/PLScoreSlot.h"

void UPLScoreWidget::PLConstruct()
{
	InfectionGameState = Cast<APLGameState_Infection>(PLGameState);
	check(InfectionGameState);

	ShowResults();
	RunCountdownTimer(InfectionGameState->GetGameData()->PostRoundTime);
}

void UPLScoreWidget::RunCountdownTimer(float InTimeRemaining)
{
	TimeRemaining = InTimeRemaining;
	ReduceTimeRemaining();
	GetWorld()->GetTimerManager().SetTimer(ReduceTimeTimerHandle, this, &UPLScoreWidget::ReduceTimeRemaining, 1.f, true);
}

void UPLScoreWidget::ReduceTimeRemaining()
{
	TimeRemaining--;
	TimeRemainingText->SetText(FText::FromString(FString::Printf(TEXT("Next round starting in %i"), TimeRemaining)));
	if (TimeRemaining <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReduceTimeTimerHandle);
		TimeRemainingText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPLScoreWidget::ShowResults()
{	
	CurrentPlayers = InfectionGameState->PlayerArray;
	//Sort the players
	CurrentPlayers.Sort([](const APlayerState& A, const APlayerState& B)
		{
			return A.GetScore() > B.GetScore();
		});
	CurrentIndex = 0;
	GetWorld()->GetTimerManager().SetTimer(ShowScoreTimerHandle, this, &UPLScoreWidget::AddWidget, 1.f, true);
}

void UPLScoreWidget::AddWidget()
{
	APLPlayerState* PLPlayerState = Cast<APLPlayerState>(CurrentPlayers[CurrentIndex]);
	UPLScoreSlot* PLScoreSlot = CreateWidget<UPLScoreSlot>(GetWorld(), ScoreSlotClass);
	PLScoreSlot->Setup(PLPlayerState->GetPlayerName(), PLPlayerState->GetScore(), PLPlayerState->GetInitialCharacterUIProfileData());
	ResultsVerticalBox->AddChildToVerticalBox(PLScoreSlot);
	CurrentIndex++;
	if (CurrentIndex >= CurrentPlayers.Num())
	{
		GetWorld()->GetTimerManager().ClearTimer(ShowScoreTimerHandle);
	}
}

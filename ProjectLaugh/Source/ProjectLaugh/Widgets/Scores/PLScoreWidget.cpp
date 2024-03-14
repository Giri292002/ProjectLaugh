// Fill out your copyright notice in the Description page of Project Settings.


#include "PLScoreWidget.h"

#include "Components/VerticalBox.h"
#include "ProjectLaugh/Core/Infection/PLInfectionGameModeData.h"
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
	InResults = InfectionGameState->GetPlayerScores();
	CurrentIndex = 0;
	GetWorld()->GetTimerManager().SetTimer(ShowScoreTimerHandle, this, &UPLScoreWidget::AddWidget, 1.f, true);
}

void UPLScoreWidget::AddWidget()
{
	FPLScoreStruct Score = InResults[CurrentIndex];
	UPLScoreSlot* PLScoreSlot = CreateWidget<UPLScoreSlot>(GetWorld(), ScoreSlotClass);
	PLScoreSlot->Setup(Score.Name, Score.Score);
	ResultsVerticalBox->AddChildToVerticalBox(PLScoreSlot);
	CurrentIndex++;
	if (CurrentIndex >= InResults.Num())
	{
		GetWorld()->GetTimerManager().ClearTimer(ShowScoreTimerHandle);
	}
}

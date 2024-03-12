// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameplayWidget.h"

#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Widgets/Shared/PLTimerWidget.h"

void UPLGameplayWidget::AddTimer(const float InSeconds, FText TimerText, bool bForward)
{
	check(TimerWidgetClass);
	UPLTimerWidget* TimerWidget = CreateWidget<UPLTimerWidget>(GetWorld(), TimerWidgetClass);
	TimerWidget->Setup(InSeconds, TimerText, bForward);
	TimerWidget->StartRunning();

	TimerVerticalBox->AddChildToVerticalBox(TimerWidget);
}

void UPLGameplayWidget::PLConstruct()
{
	PLInfectionGameState = Cast<APLGameState_Infection>(PLGameState);
	PLInfectionGameState->OnRoundUpdateDelegate.AddDynamic(this, &UPLGameplayWidget::OnRoundUpdated);
	OnRoundUpdated(PLInfectionGameState->GetCurrentRound());
}

void UPLGameplayWidget::OnRoundUpdated(int NewRound)
{
	UE_LOG(LogTemp, Warning, TEXT("Round Updated"));
	RoundTextBlock->SetText(FText::FromString(FString::FromInt(NewRound)));
}

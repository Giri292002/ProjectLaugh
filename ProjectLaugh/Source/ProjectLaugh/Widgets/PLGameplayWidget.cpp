// Fill out your copyright notice in the Description page of Project Settings.


#include "PLGameplayWidget.h"

#include "Components/VerticalBox.h"
#include "PLTimerWidget.h"

void UPLGameplayWidget::AddTimer(const float InSeconds, FText TimerText, bool bForward)
{
	check(TimerWidgetClass);
	UPLTimerWidget* TimerWidget = CreateWidget<UPLTimerWidget>(GetWorld(), TimerWidgetClass);
	TimerWidget->Setup(InSeconds, TimerText, bForward);
	TimerWidget->StartRunning();

	TimerVerticalBox->AddChildToVerticalBox(TimerWidget);
}

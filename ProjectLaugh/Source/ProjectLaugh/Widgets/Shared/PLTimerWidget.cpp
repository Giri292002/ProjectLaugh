// Fill out your copyright notice in the Description page of Project Settings.


#include "PLTimerWidget.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UPLTimerWidget::Setup(const float InSeconds, FText InTimerText, bool InbForward)
{
	bIsForward = InbForward;
	TotalSeconds = InSeconds;
	if (bIsForward)
	{
		CurrentSeconds = 0.f;
	}
	else
	{
		CurrentSeconds = InSeconds;
	}
	TimerText->SetText(InTimerText);
}

void UPLTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bIsCounting) return;

	if (bIsForward)
	{
		CurrentSeconds += InDeltaTime;
	}
	else
	{
		CurrentSeconds -= InDeltaTime;
	}

	TimerProgressBar->SetPercent(CurrentSeconds / TotalSeconds);

	if ((bIsForward && CurrentSeconds >= TotalSeconds) || (!bIsForward && CurrentSeconds <= 0.f))
	{
		RemoveFromParent();
	}
	
}

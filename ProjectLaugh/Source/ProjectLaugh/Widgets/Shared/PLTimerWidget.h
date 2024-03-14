// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Widgets/PLWidgetBase.h"
#include "PLTimerWidget.generated.h"

/**
 * A Widget class that spawns a timer for n number of seconds. It can go forward or backward, once done it will remove itself
 */

class UProgressBar;
class UTextBlock;

UCLASS()
class PROJECTLAUGH_API UPLTimerWidget : public UPLWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "PL | UI", meta = (BindWidget))
	UProgressBar* TimerProgressBar;

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI", meta = (BindWidget))
	UTextBlock* TimerText;

	UPROPERTY()
	uint8 bIsCounting : 1;

	UPROPERTY()
	uint8 bIsForward : 1;

	UPROPERTY()
	float TotalSeconds;

	UPROPERTY()
	float CurrentSeconds;

public:
	UFUNCTION(BlueprintCallable, Category = "PL | UI")
	void Setup(const float InSeconds, FText InText, bool InbForward);

	UFUNCTION()
	void StartRunning() { bIsCounting = true; }

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

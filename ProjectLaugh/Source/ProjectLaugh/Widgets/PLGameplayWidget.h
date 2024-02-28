// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Widgets/PLWidgetBase.h"
#include "PLGameplayWidget.generated.h"

class UVerticalBox;
class UPLTimerWidget;

UCLASS()
class PROJECTLAUGH_API UPLGameplayWidget : public UPLWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "PL | UI", meta = (BindWidget))
	UVerticalBox* TimerVerticalBox;

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI")
	TSubclassOf<UPLTimerWidget> TimerWidgetClass;

public:
	UFUNCTION()
	void AddTimer(const float InSeconds, FText TimerText, bool bForward = true);	
};

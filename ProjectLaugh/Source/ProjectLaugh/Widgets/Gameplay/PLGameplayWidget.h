// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Widgets/PLWidgetBase.h"
#include "PLGameplayWidget.generated.h"

class UVerticalBox;
class UPLTimerWidget;
class UTextBlock;
class APLGameState_Infection;

UCLASS()
class PROJECTLAUGH_API UPLGameplayWidget : public UPLWidgetBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "PL | UI", meta = (BindWidget))
	UVerticalBox* TimerVerticalBox;

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI")
	TSubclassOf<UPLTimerWidget> TimerWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI", meta = (BindWidget))
	UTextBlock* RoundTextBlock;

	UPROPERTY()
	APLGameState_Infection* PLInfectionGameState;

	UFUNCTION()
	void OnRoundUpdated(int NewRound);

public:
	UFUNCTION()
	void AddTimer(const float InSeconds, FText TimerText, bool bForward = true);

	virtual void PLConstruct() override;
};

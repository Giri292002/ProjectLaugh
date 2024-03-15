// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Components/TextBlock.h"
#include "ProjectLaugh/Core/Infection/PLGameState_Infection.h"
#include "ProjectLaugh/Widgets/PLWidgetBase.h"

#include "PLScoreWidget.generated.h"

class UTextBlock;
class UVerticalBox; 
class UPLScoreSlot;
class APLGameState_Infection;
class APlayerState;

UCLASS()
class PROJECTLAUGH_API UPLScoreWidget : public UPLWidgetBase
{
	GENERATED_BODY()	

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeRemainingText;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ResultsVerticalBox;

	UPROPERTY()
	APLGameState_Infection* InfectionGameState;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPLScoreSlot> ScoreSlotClass;

	UPROPERTY()
	TArray<APlayerState*> CurrentPlayers;

	UPROPERTY()
	int CurrentIndex;

	virtual void PLConstruct() override;

	UFUNCTION()
	void RunCountdownTimer(float InTimeRemaining);

	UFUNCTION()
	void ReduceTimeRemaining();

	FTimerHandle ReduceTimeTimerHandle;

	FTimerHandle ShowScoreTimerHandle;

	int TimeRemaining;

	UFUNCTION()
	void ShowResults();

	UFUNCTION()
	void AddWidget();

};

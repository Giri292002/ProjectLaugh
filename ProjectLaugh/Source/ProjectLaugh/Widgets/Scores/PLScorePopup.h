// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PLScorePopup.generated.h"

class UTextBlock;
UCLASS()
class PROJECTLAUGH_API UPLScorePopup : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* ScoreReasonText;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UTextBlock* ScoreText;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* PopupAndFadeAnimation;


public:
	UFUNCTION()
	void SetupAndPlay(FText ScoreReason, int32 Score);
	UFUNCTION()
	void AnimationFinished();
	
};

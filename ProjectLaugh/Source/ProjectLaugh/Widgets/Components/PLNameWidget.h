// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Widgets/PLWidgetBase.h"
#include "PLNameWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECTLAUGH_API UPLNameWidget : public UPLWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameTextBlock;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeInAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* FadeOutAnimation;

public:
	UFUNCTION()
	void SetName(FString InName);

	UFUNCTION()
	void FadeIn();

	UFUNCTION()
	void FadeOut();
};

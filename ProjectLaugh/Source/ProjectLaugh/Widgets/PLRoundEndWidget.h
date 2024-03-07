// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Widgets/PLWidgetBase.h"
#include "PLRoundEndWidget.generated.h"

class UTextBlock;
class UWidgetAnimation;

UCLASS()
class PROJECTLAUGH_API UPLRoundEndWidget : public UPLWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient, EditDefaultsOnly, Category = "PL | UI", meta = (BindWidgetAnim))
	UWidgetAnimation* VictoryAnimation;
	
	UPROPERTY(EditDefaultsOnly, Category = "PL | UI", meta = (BindWidget))
	UTextBlock* VictoryTextBlock;
	
	virtual void PLConstruct() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Widgets/PLWidgetBase.h"
#include "PLBigRoundWidget.generated.h"

class UTextBlock; 
UCLASS()
class PROJECTLAUGH_API UPLBigRoundWidget : public UPLWidgetBase
{
	GENERATED_BODY()

	virtual void PLConstruct() override;

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI", meta = (BindWidget))
	UTextBlock* RoundText;

	UPROPERTY(EditDefaultsOnly)
	float RemoveAfterTime = 3.f;
	
};

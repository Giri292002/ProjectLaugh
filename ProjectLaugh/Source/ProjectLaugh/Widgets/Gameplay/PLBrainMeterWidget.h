// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Widgets/PLWidgetBase.h"
#include "PLBrainMeterWidget.generated.h"

class UProgressBar;
class APLGameState_Infection;

UCLASS()
class PROJECTLAUGH_API UPLBrainMeterWidget : public UPLWidgetBase
{
	GENERATED_BODY()	

protected:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	UProgressBar* BrainMeter;

	UPROPERTY()
	APLGameState_Infection* PLInfectionGameState;

	UFUNCTION()
	void OnBrainMeterUpdated(float CurrentMeter, float MaxMeter);

	virtual void PLConstruct() override;
};

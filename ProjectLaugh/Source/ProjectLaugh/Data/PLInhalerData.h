// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PLInhalerData.generated.h"

class UPLInhalerWidget;

UCLASS()
class PROJECTLAUGH_API UPLInhalerData : public UDataAsset
{
	GENERATED_BODY()

public:
	//Lung Min is 0, Lung Max is 100, LungChargeRate is how much Lung gets charged up for 1 unit reduction of inhaler. Default value is equal to inhaler reduction rate.
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.1"))
	float LungChargeRate = 1.f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.1"))
	float LungUseRate = 1.f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.1"))
	float InhalerChargeRate = 1.f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.1"))
	float InhalerUseRate = 5.f;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0"))
	int MaxLung = 100;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0"))
	int MaxInhaler= 100;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "1.0"))
	float MaxWalkSpeedMultiplier = 1.5;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPLInhalerWidget> PLInhalerWidgetClass;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Curves/CurveFloat.h"
#include "PLScoreData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API UPLScoreData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveFloat* TimeSurvivedScoreCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveFloat* PositionScoreCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveFloat* AlphaZombieConversionScoreCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCurveFloat* BetaZombieConversionScoreCurve;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ConversionAssistScore = 50;
};

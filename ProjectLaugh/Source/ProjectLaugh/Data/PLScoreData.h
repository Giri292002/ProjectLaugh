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

	//The score that is given to alpha zombie on top of whatever is earned at the end of the round if zombies win
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ZombieWinningBonus = 500;

	//The score that is given to all elderson top of whatever is earned at the end of the round if elders win
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ElderWinningBonus = 100;
};

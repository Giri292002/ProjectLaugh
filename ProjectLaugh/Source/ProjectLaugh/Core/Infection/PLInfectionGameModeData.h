// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectLaugh/Widgets/PLRoundEndWidget.h"
#include "ProjectLaugh/Widgets/PLScoreWidget.h"
#include "ProjectLaugh/Widgets/PLBigRoundWidget.h"
#include "PLInfectionGameModeData.generated.h"

UCLASS()
class PROJECTLAUGH_API UPLInfectionGameModeData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	int32 NumberOfRounds;

	UPROPERTY(EditDefaultsOnly , meta = (Units = "Minutes"))
	float BrainMeterTime;

	UPROPERTY(EditDefaultsOnly, meta = (Units = "Seconds"))
	float PreRoundTime;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPLRoundEndWidget> RoundEndWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPLScoreWidget> ScoreWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPLBigRoundWidget> RoundWidget;
};

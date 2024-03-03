// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PLSkillCheckData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API UPLSkillCheckData : public UDataAsset
{
	GENERATED_BODY()

public:
	//The cursor that ping pongs back and forth
	UPROPERTY(EditDefaultsOnly, Category = "PL | SkillCheck", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float  SkillCheckSize = 0.02f;

	//How big the success region should be
	UPROPERTY(EditDefaultsOnly, Category = "PL | SkillCheck", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float  SkillCheckSuccessRegionSize = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "PL | SkillCheck", meta = (ClampMin = "0.0"))
	float SkillCheckSpeed = 1.f;
};

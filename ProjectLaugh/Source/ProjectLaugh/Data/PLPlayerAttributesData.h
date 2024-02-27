// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "PLPlayerAttributesData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API UPLPlayerAttributesData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "PL | Movement", meta = (ClampMin = "0.0"))
	float MaxWalkSpeed;

	//100 is default mass for physics objects in level
	UPROPERTY(EditDefaultsOnly, Category = "PL | Movement", meta = (ClampMin = "0.0"))
	float PushForce;

	UPROPERTY(EditDefaultsOnly, Category = "PL | Affiliation")
	FGameplayTag AffiliationTag;

	//Same curve is used for disappearing
	UPROPERTY(EditDefaultsOnly, Category = "PL | Appearance")
	UCurveFloat* AppearanceCurve;

	//Currently only used by zombies. Spawn in time
	UPROPERTY(EditDefaultsOnly, Category = "PL | Appearance", meta = (ClampMin = "1.0"))
	float AppearanceTime = 5.0f;

	//Currently only used by elders. Spawn out time
	UPROPERTY(EditDefaultsOnly, Category = "PL | Appearance", meta = (ClampMin = "1.0"))
	float DisappearanceTime = 1.0f;
};

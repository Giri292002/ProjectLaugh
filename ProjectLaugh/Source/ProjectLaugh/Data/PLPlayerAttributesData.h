// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PLAnimationData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API UPLAnimationData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "PL | Animation")
	UAnimMontage* ThrowMontage;
};

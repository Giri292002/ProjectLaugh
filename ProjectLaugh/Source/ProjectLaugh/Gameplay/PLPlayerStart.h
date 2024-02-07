// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PLPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API APLPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	bool HasBeenUsed() const { return bHasBeenUsed; }

	UFUNCTION()
	void SetHasBeenUsed(bool bInHasBeenUsed) { bHasBeenUsed = bInHasBeenUsed; }
protected:
	UPROPERTY(VisibleDefaultsOnly)
	bool bHasBeenUsed;
};
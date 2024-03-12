// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/SharedGameplayTags.h"
#include "GameFramework/PlayerStart.h"
#include "ProjectLaugh/Core/System/PLResetInterface.h"
#include "PLPlayerStart.generated.h"


UCLASS()
class PROJECTLAUGH_API APLPlayerStart : public APlayerStart, public IPLResetInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	bool HasBeenUsed() const { return bHasBeenUsed; }

	UFUNCTION()
	void SetHasBeenUsed(bool bInHasBeenUsed) { bHasBeenUsed = bInHasBeenUsed; }

	UPROPERTY(EditAnywhere, Category = "PL | Affilitation")
	FGameplayTag StartAffiliationTag;

	virtual void PLReset_Implementation() override;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	bool bHasBeenUsed;
};

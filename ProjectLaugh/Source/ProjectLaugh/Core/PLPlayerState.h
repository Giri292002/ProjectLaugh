// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PLPlayerState.generated.h"


UCLASS()
class PROJECTLAUGH_API APLPlayerState : public APlayerState
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
	
};

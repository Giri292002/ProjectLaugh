// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PLGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API APLGameModeBase : public AGameMode
{
	GENERATED_BODY()

	bool ReadyToStartMatch();
};

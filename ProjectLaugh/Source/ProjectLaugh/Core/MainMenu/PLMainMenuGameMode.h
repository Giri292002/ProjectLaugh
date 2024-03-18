// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PLMainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API APLMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	APLMainMenuGameMode();

	virtual void BeginPlay() override;
	
};

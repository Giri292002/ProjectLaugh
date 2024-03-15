// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "ProjectLaugh/SharedGameplayTags.h"
#include "PLCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTLAUGH_API UPLCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Exec)
	void EndRound(FString WinningTeam);
};

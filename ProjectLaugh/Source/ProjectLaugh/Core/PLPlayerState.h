// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PLPlayerState.generated.h"

class UPLScoreComponent;

UCLASS()
class PROJECTLAUGH_API APLPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	APLPlayerState();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Components")
	UPLScoreComponent* PLScoreComponent;

public:
	UPLScoreComponent* GetPLScoreComponent() const { return PLScoreComponent; }
};

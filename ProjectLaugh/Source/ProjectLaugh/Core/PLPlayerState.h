// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ProjectLaugh/Components/PLScoreComponent.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
	int32 ConversionsThisRound = 0;

public:
	UPLScoreComponent* GetPLScoreComponent() const { return PLScoreComponent; }

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_IncreaseConversion();

	int32 GetConversionsThisRound() const { return ConversionsThisRound; }
};

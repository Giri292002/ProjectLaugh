// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/SharedGameplayTags.h"
#include "ProjectLaugh/Widgets/PLWidgetBase.h"
#include "PLPlayersDisplay.generated.h"

class UHorizontalBox;
class UPLPlayerProfileSlot;
class APLGameState_Infection;
class UCharacterUIProfileData;

UCLASS()
class PROJECTLAUGH_API UPLPlayersDisplay : public UPLWidgetBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI", meta = (BindWidget))
	UHorizontalBox* ElderPlayersDisplay;

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI", meta = (BindWidget))
	UHorizontalBox* ZombiePlayersDisplay;

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI")
	TSubclassOf<UPLPlayerProfileSlot> PlayerProfileWidgetClass;

	UPROPERTY()
	APLGameState_Infection* InfectionGameState;

	virtual void PLConstruct() override;

	UFUNCTION()
	void RefreshCharacterDisplay();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PLMainMenuHUD.generated.h"

class UPLMainMenuWidget;
UCLASS()
class PROJECTLAUGH_API APLMainMenuHUD : public AHUD
{
	GENERATED_BODY()

	virtual void BeginPlay() override;	

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UPLMainMenuWidget> MainMenuWidgetClass;

private:
	UPLMainMenuWidget* MainMenuWidget;
};

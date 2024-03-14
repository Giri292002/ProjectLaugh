// Fill out your copyright notice in the Description page of Project Settings.


#include "PLMainMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectLaugh/Widgets/MainMenu/PLMainMenuWidget.h"

void APLMainMenuHUD::BeginPlay()
{
	if (!ensureAlwaysMsgf(MainMenuWidgetClass, TEXT("Main Menu Widget Class is Invalid. Please set it up in HUD")))
	{
		return;
	}
	MainMenuWidget = CreateWidget<UPLMainMenuWidget>(GetWorld(), MainMenuWidgetClass, FName(TEXT("MainMenu")));
	MainMenuWidget->AddToViewport();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (ensureAlwaysMsgf(PC, TEXT("Invalid Player controller")))
	{
		PC->SetShowMouseCursor(true);
		PC->SetInputMode(FInputModeUIOnly());
	}
}

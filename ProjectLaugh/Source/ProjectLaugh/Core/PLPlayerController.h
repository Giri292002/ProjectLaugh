// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PLPlayerController.generated.h"

class UPLWaitingForPlayersWidget;
class ULevelSequence;
class ULevelSequencePlayer;
class UInputMappingContext;
class UPLGameplayWidget;

UCLASS()
class PROJECTLAUGH_API APLPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_DrawWaitingForPlayersWidget();

	UFUNCTION(BlueprintCallable, Client, Reliable)
	void Client_RemoveWaitingForPlayersWidget();

	UFUNCTION(Client, Reliable)
	virtual void Client_AddComponentWidgets();

	UFUNCTION(Client, Reliable)
	virtual void Client_RemoveComponentWidgets();

	//Adds a timer widget that removes itself once done
	UFUNCTION(Client, Reliable)
	void Client_AddTimer(float InSeconds, const FText& InTimerText, bool InbForward);

	UPROPERTY(Replicated)
	FRotator RepPlayerControllerRotation;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | UI")
	TSubclassOf<UPLWaitingForPlayersWidget> PLWaitingForPlayersWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "PL | UI")
	UPLWaitingForPlayersWidget* PLWaitingForPlayersWidget;

	UPROPERTY(EditDefaultsOnly, Category = "PL | UI")
	TSubclassOf<UPLGameplayWidget> PLGameplayWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "PL | UI")
	UPLGameplayWidget* PLGameplayWidget;

	//TODO: Move this to a data table so we have different waiting sequence for different maps
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Cinematics")
	ULevelSequence* WaitingSequence;

	UPROPERTY(BlueprintReadOnly, Category = "PL | Cinematics")
	ULevelSequencePlayer* LevelSequencePlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Input")
	UInputMappingContext* DefaultMappingContext;

	UFUNCTION()
	void PlayWaitingCinematicSequence();
	
	UFUNCTION()
	void StoPlayingWaitingCinematicSequence();

	virtual void BeginPlay() override;
	virtual void OnNetCleanup(UNetConnection* Connection) override;
	virtual void AcknowledgePossession(class APawn* NewPawn) override;
	virtual void Tick(float DeltaSeconds) override;
};
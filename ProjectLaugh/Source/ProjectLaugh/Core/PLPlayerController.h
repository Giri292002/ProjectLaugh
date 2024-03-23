// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectLaugh/SharedGameplayTags.h"
#include "PLPlayerController.generated.h"

class UPLWaitingForPlayersWidget;
class ULevelSequence;
class ULevelSequencePlayer;
class UInputMappingContext;
class UPLGameplayWidget;
class UPLWidgetBase;
class APLPlayerCharacter;

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
	void Client_DrawGameplayWidget();

	UFUNCTION(Client, Reliable)
	virtual void Client_AddComponentWidgets();

	UFUNCTION(Client, Reliable)
	virtual void Client_RemoveComponentWidgets();

	UFUNCTION(Client, Reliable)
	void Client_AddPLWidget(TSubclassOf<UPLWidgetBase> WidgetClassToAdd);

	UFUNCTION(Client, Reliable) 
	void Client_RemovePLWidget(TSubclassOf<UPLWidgetBase> WidgetClassToRemove);

	//Spawns a widget and also calls setup component from here
	UFUNCTION(Client, Reliable)
	void Client_AddComponentWidget(TSubclassOf<UPLComponentWidgetBase> WidgetClassToAdd, UPLActorComponent* InComp);

	UFUNCTION(Client, Reliable)
	void Client_RemoveAllWidgets();

	//Adds a timer widget that removes itself once done
	UFUNCTION(Client, Reliable)
	void Client_AddTimer(float InSeconds, const FText& InTimerText, bool InbForward);

	UFUNCTION(Client, Reliable)
	void ToggleDisableInput(bool bDisable);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCurrentAffiliationTag(FGameplayTag AffiliationTag);

	UFUNCTION(BlueprintCallable)
	FGameplayTag GetCurrentAffiliationTag() const { return CurrentAffilitationTag; }

	UFUNCTION(Client, Reliable)
	void Client_PlayResultCinematicSequence();

	UFUNCTION(Client, Reliable)
	void Client_OpenMainMenu(); 

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Cinematics")
	ULevelSequence* ResultSequence;

	UPROPERTY(BlueprintReadOnly, Category = "PL | Cinematics")
	ULevelSequencePlayer* LevelSequencePlayer;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PL | Affiliation")
	FGameplayTag CurrentAffilitationTag;

	UPROPERTY()
	TSet<UPLWidgetBase*> SpawnedWidgets;

	UPROPERTY(Replicated)
	APLPlayerCharacter* ControllingPLPlayerCharacter;

	UFUNCTION()
	void PlayWaitingCinematicSequence();
	
	UFUNCTION()
	void StoPlayingWaitingCinematicSequence();

	virtual void BeginPlay() override;
	virtual void OnNetCleanup(UNetConnection* Connection) override;
	virtual void AcknowledgePossession(class APawn* NewPawn) override;
	virtual void Tick(float DeltaSeconds) override;
private:
	template<typename T>
	T* Internal_AddWidget(TSubclassOf<T> WidgetClassToAdd);
};

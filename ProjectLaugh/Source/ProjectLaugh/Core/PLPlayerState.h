// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ProjectLaugh/Core/System/PLResetInterface.h"
#include "ProjectLaugh/Components/PLScoreComponent.h"
#include "PLPlayerState.generated.h"

class UPLScoreComponent;
class UCharacterUIProfileData;
class APLPlayerCharacter;

UCLASS()
class PROJECTLAUGH_API APLPlayerState : public APlayerState, public IPLResetInterface
{
	GENERATED_BODY()

protected:

	APLPlayerState();

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Components")
	UPLScoreComponent* PLScoreComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
	int32 ConversionsThisRound = 0;

	UPROPERTY(Replicated)
	UCharacterUIProfileData* CurrentCharacterUIData;

	UPROPERTY(Replicated)
	UCharacterUIProfileData* InitialCharacterUIData;


	/* Stores information of what elder character this player state played as
	*  Will be empty the first round, but will have information in the subsequent rounds
	*/
	UPROPERTY(Replicated)
	TSubclassOf<APLPlayerCharacter> ElderCharacterClass;

public:
	UPLScoreComponent* GetPLScoreComponent() const { return PLScoreComponent; }

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_IncreaseConversion();

	int32 GetConversionsThisRound() const { return ConversionsThisRound; }

	UFUNCTION(BlueprintCallable)
	UCharacterUIProfileData* GetCurrentCharacterUIProfileData() const { return CurrentCharacterUIData; };

	UFUNCTION(BlueprintCallable)
	UCharacterUIProfileData* GetInitialCharacterUIProfileData() const { return InitialCharacterUIData; }

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetCharacterUIProfileData(UCharacterUIProfileData* InUIProfileData);

	UFUNCTION()
	void SetElderCharacterClass(TSubclassOf<APLPlayerCharacter> InElderClass) { ElderCharacterClass = InElderClass; }

	UFUNCTION()
	TSubclassOf<APLPlayerCharacter> GetElderCharacterClass() const { return ElderCharacterClass; }

	virtual void PLReset_Implementation() override;

};

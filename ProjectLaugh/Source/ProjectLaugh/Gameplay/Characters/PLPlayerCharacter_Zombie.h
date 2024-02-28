// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectLaugh/Core/PLPlayerCharacter.h"
#include "PLPlayerCharacter_Zombie.generated.h"

class UTimelineComponent;
class UCurveFloat;
class UPLZombieAttributesData;

UCLASS()
class PROJECTLAUGH_API APLPlayerCharacter_Zombie : public APLPlayerCharacter
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PL | Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* PounceAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PL | Pounce")
	float PounceCooldownTime;

	APLPlayerCharacter_Zombie();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Client, Unreliable)
	void Net_Pounce();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void Server_Pounce(FRotator NewRotation, FHitResult HitResult);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SpawnZombie();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SpawnZombie();

	UFUNCTION()
	void Server_OnPounceCooldownFinished();

	FTimerHandle PounceCooldownTimer;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Restart() override;

	virtual void AppearanceTimelineFinishedCallback() override;	
};
